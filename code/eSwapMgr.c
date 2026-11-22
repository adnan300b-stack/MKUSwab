#include "eSwapMgr.h"
#include "config.h"
#include "mkstructs.h"
#include "mkuplrinfo.h"
#include "mkdeception.h"
#include "eLog.h"

#include <pspsdk.h>
#include <pspkernel.h>
#include <string.h>
#include <ctype.h>

static struct char_swap TheSwaps[MAX_SWAPS];
static int iSwapCount = 0;

// ---------------------------------------------------------------
// helpers
// ---------------------------------------------------------------

// case-insensitive prefix compare
static int starts_with(const char* s, const char* prefix)
{
	while (*prefix)
	{
		if (tolower((unsigned char)*s) != tolower((unsigned char)*prefix))
			return 0;
		s++;
		prefix++;
	}
	return 1;
}

// copy a token and normalize it: uppercase + keep only A-Z 0-9
// so "Sub-Zero" / "SUB ZERO" / "subzero" all become "SUBZERO"
static void normalize_name(const char* src, char* dst, int maxLen)
{
	int n = 0;
	while (*src && n < maxLen - 1)
	{
		char c = *src++;
		if (isalpha((unsigned char)c))
			dst[n++] = (char)toupper((unsigned char)c);
		else if (isdigit((unsigned char)c))
			dst[n++] = c;
		// anything else (space, -, ', _) is skipped
	}
	dst[n] = 0;
}

// find a character id by (normalized) name, -1 if unknown
static int find_character(const char* name)
{
	char wanted[64];
	normalize_name(name, wanted, sizeof(wanted));

	if (wanted[0] == 0)
		return -1;

	for (int i = 0; i < TOTAL_CHARACTERS; i++)
	{
		char current[64];
		normalize_name(pCharTable[i].name, current, sizeof(current));
		if (strcmp(wanted, current) == 0)
			return i;
	}
	return -1;
}

// trim spaces/tabs around a token in-place
static void trim_token(char* str)
{
	char* start = str;
	while (*start == ' ' || *start == '\t' || *start == '\r' || *start == '\n')
		start++;

	char* end = start + strlen(start);
	while (end > start && (end[-1] == ' ' || end[-1] == '\t' || end[-1] == '\r' || end[-1] == '\n'))
		*--end = 0;

	if (start != str)
		memmove(str, start, strlen(start) + 1);
}

// ---------------------------------------------------------------
// parsing
// ---------------------------------------------------------------

static int parse_chang_line(char* line)
{
	// expected: chang(NAME1->NAME2)
	char* open  = strchr(line, '(');
	char* arrow = strstr(line, "->");
	char* close = strchr(line, ')');

	if (!open || !arrow || !close || arrow < open || close < arrow)
	{
		mk_sprintf(msgBuff, "ERROR: bad chang() syntax, ignoring: %s\n", line);
		LOG_Message(msgBuff);
		return 0;
	}

	// visual name: between '(' and '->'
	*arrow = 0;
	char* visual_name = open + 1;
	trim_token(visual_name);

	// selected name: between '->' and ')'
	*close = 0;
	char* selected_name = arrow + 2;
	trim_token(selected_name);

	int visual_id   = find_character(visual_name);
	int selected_id = find_character(selected_name);

	if (visual_id < 0)
	{
		mk_sprintf(msgBuff, "ERROR: unknown character '%s', ignoring line: %s\n", visual_name, line);
		LOG_Message(msgBuff);
		return 0;
	}
	if (selected_id < 0)
	{
		mk_sprintf(msgBuff, "ERROR: unknown character '%s', ignoring line: %s\n", selected_name, line);
		LOG_Message(msgBuff);
		return 0;
	}

	if (iSwapCount >= MAX_SWAPS)
	{
		mk_sprintf(msgBuff, "ERROR: swap limit (%d) reached, ignoring line: %s\n", MAX_SWAPS, line);
		LOG_Message(msgBuff);
		return 0;
	}

	TheSwaps[iSwapCount].visual_id   = visual_id;
	TheSwaps[iSwapCount].selected_id = selected_id;
	strncpy(TheSwaps[iSwapCount].raw, line, sizeof(TheSwaps[iSwapCount].raw) - 1);
	TheSwaps[iSwapCount].raw[sizeof(TheSwaps[iSwapCount].raw) - 1] = 0;
	iSwapCount++;

	mk_sprintf(msgBuff, "INFO: registered swap: pick %s (%d) -> loads %s (%d)\n",
		pCharTable[selected_id].name, selected_id,
		pCharTable[visual_id].name, visual_id);
	LOG_Message(msgBuff);

	return 1;
}

// ---------------------------------------------------------------
// public api
// ---------------------------------------------------------------

int SwapMgr_Init()
{
	iSwapCount = 0;

	char path[256];
	mk_sprintf(path, "%s/Settings.ini", PLUGIN_DIR);

	SceUID fd = sceIoOpen(path, PSP_O_RDONLY, 0);
	if (fd < 0)
	{
		mk_sprintf(msgBuff, "ERROR: could not open %s (no swaps loaded)\n", path);
		LOG_Message(msgBuff);
		return 0;
	}

	int size = sceIoLseek(fd, 0, PSP_SEEK_END);
	sceIoLseek(fd, 0, PSP_SEEK_SET);

	static char buffer[16384];
	if (size > (int)sizeof(buffer) - 1)
		size = sizeof(buffer) - 1;

	int got = sceIoRead(fd, buffer, size);
	sceIoClose(fd);

	if (got <= 0)
	{
		mk_sprintf(msgBuff, "ERROR: %s is empty (no swaps loaded)\n", path);
		LOG_Message(msgBuff);
		return 0;
	}
	buffer[got] = 0;

	mk_sprintf(msgBuff, "INFO: reading %s (%d bytes)\n", path, got);
	LOG_Message(msgBuff);

	// split into lines and look for chang(...)
	char* p = buffer;
	while (*p)
	{
		char* line_end = strchr(p, '\n');
		if (line_end)
			*line_end = 0;

		trim_token(p);

		// skip empty lines and comments (; or #)
		if (p[0] && p[0] != ';' && p[0] != '#')
		{
			if (starts_with(p, "chang"))
				parse_chang_line(p);
			else
			{
				mk_sprintf(msgBuff, "WARNING: unknown directive, ignoring: %s\n", p);
				LOG_Message(msgBuff);
			}
		}

		if (!line_end)
			break;
		p = line_end + 1;
	}

	mk_sprintf(msgBuff, "INFO: SwapMgr_Init done, %d swap(s) active\n", iSwapCount);
	LOG_Message(msgBuff);
	return iSwapCount;
}

// returns the model/anim character id for a picked character id
int SwapMgr_GetVisual(int selected_id)
{
	for (int i = 0; i < iSwapCount; i++)
	{
		if (TheSwaps[i].selected_id == selected_id)
			return TheSwaps[i].visual_id;
	}
	return selected_id;
}

int SwapMgr_Count()
{
	return iSwapCount;
}

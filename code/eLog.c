#include "eLog.h"
#include "config.h"
#include "mkdeception.h"
#include <string.h>

#include <pspsdk.h>
#include <pspkernel.h>

static char log_path[256];
static int log_uid = -1;

void LOG_Initialise()
{
	mk_sprintf(log_path, "%s/logs.log", PLUGIN_DIR);

	log_uid = sceIoOpen(log_path, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_TRUNC, 0777);
	if (log_uid >= 0) sceIoClose(log_uid);

	LOG_Message("==============================================\n");
	LOG_Message("MKUSwap 1.0 - Mortal Kombat Unchained (ULUS10102)\n");
	LOG_Message("Character model/moves swap plugin\n");
	LOG_Message("==============================================\n");
}

void LOG_Message(char* message)
{
	if (log_uid < 0 && log_path[0] == 0)
		return;

	log_uid = sceIoOpen(log_path, PSP_O_WRONLY | PSP_O_CREAT | PSP_O_APPEND, 0777);
	if (log_uid < 0)
		return;

	sceIoWrite(log_uid, message, strlen(message));
	sceIoClose(log_uid);
}

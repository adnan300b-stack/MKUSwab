#include "mkuplrinfo.h"
#include "eLog.h"
#include "eSwapMgr.h"
#include "mkdeception.h"
#include "pspmem.h"

#include <string.h>
#include <stdio.h>

int last_created_player = PLAYER1;
int last_saved_char_id = PLAYER1;

// ---------------------------------------------------------------
// file tables for MKDA_MOKAP / MKDA_KANO (restored leftover files)
// ---------------------------------------------------------------

struct mk_toc_entry mokap_toc[] = {
	{"",0,0 },
	{"",0,498304},
	{"",0,498304},
	{"",0,145792},
	{"",0,2720},
	{"",0,21600},
	{"",0,12224},
	{"",0,16032},
	{"",0,9600},
	{"",0,14592},
	{"",0,9600},
	{"",0,15424},
	{"",0,9600},
	{"",0,21120},
	{"",0,52352},
	{"",0,170496},
	{"",0,195328},
	{"",0,222208},
	{0,0,0}
};

struct mk_file_entry mokap_file_entry_table[] = {
	{"mkda_mokap.ssf"	,0, 4},
	{"mkda_mokap.sec"	,0, 1},
	{"mkda_mokap_ap.sec"	,0, 1},
	{"sh_mkda_mokap.sec"	,0, 1},
	{"mkda_mokap.mko"	,0, 3},
	{"blood_fx.mko"	,0, 3},
	{"fight_fx.mko"	,0, 3},
	{"karate.mko"	,0, 3},
	{"style_karate.sec"	,0,1 },
	{"jeetkunedo.mko"	,0, 3},
	{"style_jeet_kun_do.sec",0,1 },
	{"nunchaku.mko"	,0, 3},
	{"style_nunchaku.sec"	, 0,1 },
	{"mokap_anims.sec"	,0, 2},
	{"pz_jax_anims.sec"	,0, 2},
	{"karate_anims.sec"	,0, 2},
	{"jeetkunedo_anims.sec"	,0, 2},
	{"nunchaku_anims.sec"	,0, 2},
};

struct mk_toc_entry mokap_file_table[] = {
	{&mokap_file_entry_table[0]	,0,0 },
	{&mokap_file_entry_table[1]	,0,498304},
	{&mokap_file_entry_table[2]	,0,498304},
	{&mokap_file_entry_table[3]	,0,145792},
	{&mokap_file_entry_table[4]	,0,2720},
	{&mokap_file_entry_table[5]	,0,21600},
	{&mokap_file_entry_table[6]	,0,12224},
	{&mokap_file_entry_table[7]	,0,16032},
	{&mokap_file_entry_table[8]	,0,9600},
	{&mokap_file_entry_table[9]	,0,14592},
	{&mokap_file_entry_table[10]	,0,9600},
	{&mokap_file_entry_table[11]	,0,15424},
	{&mokap_file_entry_table[12]	,0,9600},
	{&mokap_file_entry_table[13]	,0,21120},
	{&mokap_file_entry_table[14]	,0,52352},
	{&mokap_file_entry_table[15]	,0,170496},
	{&mokap_file_entry_table[16]	,0,195328},
	{&mokap_file_entry_table[17]	,0,222208},
	{0,0,0}
};

struct mk_toc_entry kano_toc[] = {
	{"",0,0 },
	{"",0,483968},
	{"",0,483968},
	{"",0,145792},
	{"",0,2688},
	{"",0,21600},
	{"",0,12224},
	{"",0,16032},
	{"",0,9600},
	{"",0,14592},
	{"",0,9600},
	{"",0,15424},
	{"",0,9600},
	{"",0,27008},
	{"",0,52352},
	{"",0,170496},
	{"",0,195328},
	{"",0,222208},
	{0,0,0}
};

struct mk_file_entry kano_file_entry_table[] = {
	{"mkda_kano.ssf"	,0, 4},
	{"mkda_kano.sec"	,0, 1},
	{"mkda_kano_ap.sec"	,0, 1},
	{"sh_mkda_kano.sec"	,0, 1},
	{"mkda_kano.mko"	,0, 3},
	{"blood_fx.mko"	,0, 3},
	{"fight_fx.mko"	,0, 3},
	{"karate.mko"	,0, 3},
	{"style_karate.sec"	,0,1 },
	{"jeetkunedo.mko"	,0, 3},
	{"style_jeet_kun_do.sec",0,1 },
	{"nunchaku.mko"	,0, 3},
	{"style_nunchaku.sec"	, 0,1 },
	{"kano_anims.sec"	,0, 2},
	{"pz_jax_anims.sec"	,0, 2},
	{"karate_anims.sec"	,0, 2},
	{"jeetkunedo_anims.sec"	,0, 2},
	{"nunchaku_anims.sec"	,0, 2},
};

struct mk_toc_entry kano_file_table[] = {
	{&kano_file_entry_table[0]	,0,0 },
	{&kano_file_entry_table[1]	,0,483968},
	{&kano_file_entry_table[2]	,0,483968},
	{&kano_file_entry_table[3]	,0,145792},
	{&kano_file_entry_table[4]	,0,2688},
	{&kano_file_entry_table[5]	,0,21600},
	{&kano_file_entry_table[6]	,0,12224},
	{&kano_file_entry_table[7]	,0,16032},
	{&kano_file_entry_table[8]	,0,9600},
	{&kano_file_entry_table[9]	,0,14592},
	{&kano_file_entry_table[10]	,0,9600},
	{&kano_file_entry_table[11]	,0,15424},
	{&kano_file_entry_table[12]	,0,9600},
	{&kano_file_entry_table[13]	,0,27008},
	{&kano_file_entry_table[14]	,0,52352},
	{&kano_file_entry_table[15]	,0,170496},
	{&kano_file_entry_table[16]	,0,195328},
	{&kano_file_entry_table[17]	,0,222208},
	{0,0,0}
};

// ---------------------------------------------------------------
// character table (name, model file table, alt table, script)
// ---------------------------------------------------------------

struct char_info_entry pCharTable[] = {
	{"SCORPION"	, (int*)0x8B05CA4	, (int*)0x8B05DD0	,"scorpion.mko"},
	{"BARAKA"	, (int*)0x8B00560	, (int*)0x8B00698	,"baraka.mko"},
	{"NOOB"	, (int*)0x8B055B4	, (int*)0x8B05680	,"noob.mko"},
	{"SUB-ZERO"	, (int*)0x8B06EB0	, (int*)0x8B06FE8	,"subzero.mko"},
	{"MILEENA"	, (int*)0x8B03F74	, (int*)0x8B040AC	,"mileena.mko"},
	{"NIGHTWOLF"	, (int*)0x8B05308	, (int*)0x8B05440	,"nightwolf.mko"},
	{"ERMAC"	, (int*)0x8B014D8	, (int*)0x8B0161C	,"ermac.mko"},
	{"ASHRAH"	, (int*)0x8B0023C	, (int*)0x8B00374	,"ashrah.mko"},
	{"SINDEL"	, (int*)0x8B06694	, (int*)0x8B067CC	,"sindel.mko"},
	{"LI MEI"	, (int*)0x8B03944	, (int*)0x8B0380C	,"limei.mko"},
	{"BO' RAI CHO"	, (int*)0x8B00A40	, (int*)0x8B00B78	,"boraicho.mko"},
	{"HOTARU"	, (int*)0x8B01FB8	, (int*)0x8B020F0	,"hotaru.mko"},
	{"KENSHI"	, (int*)0x8B02BE8	, (int*)0x8B02D20	,"kenshi.mko"},
	{"SMOKE"	, (int*)0x8B06C10	, (int*)0x8B06D0C	,"smoke.mko"},
	{"HAVIK"	, (int*)0x8B06964	, (int*)0x8B06A9C	,"skab.mko"},
	{"TANYA"	, (int*)0x8B071BC	, (int*)0x8B072F4	,"tanya.mko"},
	{"LIU KANG"	, (int*)0x8B03C5C	, (int*)0x8B03B24	,"liukang.mko"},
	{"GHOST"	, (int*)0x8B03D94	, (int*)0x8B03D94	,"liukang_ghost.mko"},
	{"KIRA"	, (int*)0x8B02EDC	, (int*)0x8B03014	,"kira.mko"},
	{"KABAL"	, (int*)0x8B028E8	, (int*)0x8B02A20	,"kabal.mko"},
	{"KOBRA"	, (int*)0x8B034E8	, (int*)0x8B03620	,"kobra.mko"},
	{"JADE"	, (int*)0x8B022DC	, (int*)0x8B02414	,"jade.mko"},
	{"DAIROU"	, (int*)0x8B0107C	, (int*)0x8B011B4	,"dairou.mko"},
	{"RAIDEN"	, (int*)0x8B05A04	, (int*)0x8B05B3C	,"raiden.mko"},
	{"DARRIUS"	, (int*)0x8B00D58	, (int*)0x8B00E90	,"cassius.mko"},
	{"SHUJINKO"	, (int*)0x8B06268	, (int*)0x8B063A0	,"shujinko.mko"},
	{"SHUJINKO"	, (int*)0x8B064E4	, (int*)0x8B064E4	,"shujinko_13.mko"},
	{"NOOB - SMOKE"	, (int*)0x8B058D8	, (int*)0x8B057B8	,"noobsmoke.mko"},
	{"MONSTER"	, (int*)0x8B017CC	, (int*)0x8B017CC	,"freak.mko"},
	{"ONAGA"	, (int*)0x8B01340	, (int*)0x8B01340	,"dragonking.mko"},
	{"GORO"	, (int*)0x8B01CAC	, (int*)0x8B01DD8	,"goro.mko"},
	{"SHAO KAHN"	, (int*)0x8B05FB0	, (int*)0x8B060DC	,"shaokahn.mko"},
	{"FROST"	, (int*)0x8B01994	, (int*)0x8B01ACC	,"frost.mko"},
	{"KITANA"	, (int*)0x8B031DC	, (int*)0x8B03308	,"kitana.mko"},
	{"JAX"	, (int*)0x8B025E8	, (int*)0x8B02714	,"jax.mko"},
	{"BLAZE"	, (int*)0x8B0086C	, (int*)0x8B0086C	,"blaze.mko"},
	{"RANDOM"	, (int*)0x8B05FB0	, (int*)0x8B060DC	,"shaokahn.mko"},
	{"JAX"	, (int*)0x8B04688	, (int*)0x8B04790	,"mkda_jax.mko"},
	{"RAIDEN"	, (int*)0x8B04EC4	, (int*)0x8B04EC4	,"mkda_rayden.mko"},
	{"QUAN CHI"	, (int*)0x8B04D08	, (int*)0x8B04D08	,"mkda_quanchi.mko"},
	{"KUNG LAO"	, (int*)0x8B04A64	, (int*)0x8B04A64	,"mkda_kunglao.mko"},
	{"JOHNNY CAGE"	, (int*)0x8B0431C	, (int*)0x8B0431C	,"mkda_cage.mko"},
	{"SONYA"	, (int*)0x8B05164	, (int*)0x8B05164	,"mkda_sonya.mko"},
	{"NITARA"	, (int*)0x8B04BB8	, (int*)0x8B04BB8	,"mkda_nitara.mko"},
	{"SHANG TSUNG"	, (int*)0x8B05038	, (int*)0x8B05038	,"mkda_shang.mko"},
	{"FROST"	, (int*)0x8B04568	, (int*)0x8B04568	,"mkda_frost.mko"},
	{"KITANA"	, (int*)0x8B048CC	, (int*)0x8B048CC	,"mkda_kitana.mko"},
	{"DRAHMIN"	, (int*)0x8B0446C	, (int*)0x8B0446C	,"mkda_drahmin.mko"},
	{"BLAZE"	, (int*)0x8B041F0	, (int*)0x8B041F0	,"mkda_blaze.mko"},
	{"MOKAP"	, (int*)&mokap_file_table	, (int*)&mokap_file_table	,"mkda_mokap.mko"},
	{"KANO"	, (int*)&kano_file_table	, (int*)&kano_file_table	,"mkda_kano.mko"},
};

// ---------------------------------------------------------------
// player creation hook
// ---------------------------------------------------------------

int custom_create_player(int plr, struct player_info* obj)
{
	last_created_player = plr;

	mk_sprintf(msgBuff, "INFO: create player %d, picked character %d (%s)\n",
		plr, obj->characterID, pCharTable[obj->characterID].name);
	LOG_Message(msgBuff);

	if (SwapMgr_GetVisual(obj->characterID) != obj->characterID)
	{
		mk_sprintf(msgBuff, "INFO: swap active -> model/anims of %s (%d), specials of %s\n",
			pCharTable[SwapMgr_GetVisual(obj->characterID)].name,
			SwapMgr_GetVisual(obj->characterID),
			pCharTable[obj->characterID].name);
		LOG_Message(msgBuff);
	}

	return create_plr(plr, obj);
}

// ---------------------------------------------------------------
// model/anims loading hooks  -> swapped to the VISUAL character
// (this is what changes the appearance and the basic attacks)
// ---------------------------------------------------------------

void custom_load_ssf(struct mem_ssf_header* ptr)
{
	int p1_id = ReadInt(PLAYER1_ADDR + 0x54);
	int p2_id = ReadInt(PLAYER2_ADDR + 0x54);

	int picked = (last_created_player == PLAYER2) ? p2_id : p1_id;
	int visual = SwapMgr_GetVisual(picked);

	mk_sprintf(msgBuff, "INFO: load_ssf (model/anims) player %d: picked %d -> visual %d\n",
		last_created_player, picked, visual);
	LOG_Message(msgBuff);

	load_ssf((struct mem_ssf_header*)pCharTable[visual].file_table);
}

void custom_load_ssf_alt(struct mem_ssf_header* ptr)
{
	int p1_id = ReadInt(PLAYER1_ADDR + 0x54);
	int p2_id = ReadInt(PLAYER2_ADDR + 0x54);

	int picked = (last_created_player == PLAYER2) ? p2_id : p1_id;
	int visual = SwapMgr_GetVisual(picked);

	mk_sprintf(msgBuff, "INFO: load_ssf (alt model) player %d: picked %d -> visual %d\n",
		last_created_player, picked, visual);
	LOG_Message(msgBuff);

	load_ssf((struct mem_ssf_header*)pCharTable[visual].alt_file_table);
}

void custom_load_ssf_async(struct mem_ssf_header* ptr)
{
	int async_proc_data = ReadInt(0x58F6B4);
	int charID = *(int*)(async_proc_data + 12);
	int flags  = *(int*)(async_proc_data + 16);

	int visual = SwapMgr_GetVisual(charID);

	mk_sprintf(msgBuff, "INFO: load_ssf (async) char %d -> visual %d (flags %x)\n",
		charID, visual, flags);
	LOG_Message(msgBuff);

	if (flags & 1)
		load_ssf((struct mem_ssf_header*)pCharTable[visual].alt_file_table);
	else
		load_ssf((struct mem_ssf_header*)pCharTable[visual].file_table);
}

// select screen helper loads
void hook_unk_func(int unk, int id)
{
	last_saved_char_id = id;
	func_6F388(unk, id);
}

void unk_load_ssf(struct mem_ssf_header* ptr)
{
	int visual = SwapMgr_GetVisual(last_saved_char_id);

	mk_sprintf(msgBuff, "INFO: load_ssf (select) char %d -> visual %d\n",
		last_saved_char_id, visual);
	LOG_Message(msgBuff);

	load_ssf((struct mem_ssf_header*)pCharTable[visual].alt_file_table);
}

// ---------------------------------------------------------------
// script loading hooks -> NOT swapped on purpose!
// the .mko script contains the special moves (projectiles, etc.)
// so it stays the one of the PICKED character.
// If you want a full moveset swap too, change 'charID' to
// SwapMgr_GetVisual(charID) in the two functions below.
// ---------------------------------------------------------------

int custom_load_script_data(int id, char* name)
{
	int p1_id = ReadInt(PLAYER1_ADDR + 0x54);
	int p2_id = ReadInt(PLAYER2_ADDR + 0x54);

	int picked = (last_created_player == PLAYER2) ? p2_id : p1_id;

	mk_sprintf(msgBuff, "INFO: load_script (specials) player %d: script of picked char %d (%s)\n",
		last_created_player, picked, pCharTable[picked].scriptName);
	LOG_Message(msgBuff);

	return load_script_data(id, pCharTable[picked].scriptName);
}

int custom_load_script_data_async(int id, char* name)
{
	int async_proc_data = ReadInt(0x58F6B4);
	int charID = *(int*)(async_proc_data + 12);

	mk_sprintf(msgBuff, "INFO: load_script (async, specials) char %d (%s)\n",
		charID, pCharTable[charID].scriptName);
	LOG_Message(msgBuff);

	return load_script_data(id, pCharTable[charID].scriptName);
}

int unk_load_script_data(int id, char* name)
{
	mk_sprintf(msgBuff, "INFO: load_script (select) char %d (%s)\n",
		last_saved_char_id, pCharTable[last_saved_char_id].scriptName);
	LOG_Message(msgBuff);

	return load_script_data(id, pCharTable[last_saved_char_id].scriptName);
}

// ---------------------------------------------------------------
// HUD names -> show the VISUAL character name
// ---------------------------------------------------------------

int player1_name(int id, int font, char* text, int x, int y, int unk)
{
	int p1_id = ReadInt(PLAYER1_ADDR + 0x54);
	int visual = SwapMgr_GetVisual(p1_id);
	return string_left_xy(id, font, pCharTable[visual].name, x, y, unk);
}

int player2_name(int id, int font, char* text, int y, int x, int unk)
{
	int p2_id = ReadInt(PLAYER2_ADDR + 0x54);
	int visual = SwapMgr_GetVisual(p2_id);
	return string_right_xy(id, font, pCharTable[visual].name, y, x, unk);
}

void set_win_name(char* buf, char* format, char* name)
{
	int round_winner = ReadInt(0x2F3D04);

	int p1_id = ReadInt(PLAYER1_ADDR + 0x54);
	int p2_id = ReadInt(PLAYER2_ADDR + 0x54);

	if (round_winner == 2)
		sprintf(buf, format, pCharTable[SwapMgr_GetVisual(p2_id)].name);
	else
		sprintf(buf, format, pCharTable[SwapMgr_GetVisual(p1_id)].name);
}

// ---------------------------------------------------------------
// custom toc setup for MOKAP / KANO file tables
// ---------------------------------------------------------------

void calculate_custom_toc()
{
	for (int i = 0; i < 18; i++)
	{
		mokap_file_entry_table[i].belong = &mokap_file_table;
		kano_file_entry_table[i].belong = &kano_file_table;
	}

	int baseSize = 2048;
	for (int i = 1; i < 18; i++)
	{
		mokap_file_table[i].previousSize = baseSize;
		baseSize += (mokap_file_table[i].size + 2048 - 1) & -2048;
	}

	baseSize = 2048;
	for (int i = 1; i < 18; i++)
	{
		kano_file_table[i].previousSize = baseSize;
		baseSize += (kano_file_table[i].size + 2048 - 1) & -2048;
	}
}

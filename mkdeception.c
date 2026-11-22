#include "mkdeception.h"
#include "pspmem.h"
#include "eLog.h"
#include <stdio.h>

char msgBuff[256];

void(*mk_sprintf)(char* ptr, char* fmt, ...);
void(*load_ssf)(struct mem_ssf_header* ssf);
int (*load_script_data)(int id, char* name);
int (*create_plr)(int plr, struct player_info* obj);
int (*string_right_xy)(int id, int font, char* text, int y, int x, int unk);
int (*string_left_xy)(int id, int font, char* text, int y, int x, int unk);
void(*func_6F388)(int unk, int id);

void MKDeception_Init(unsigned int addr)
{
	mk_sprintf       = (void*)(addr + 0x248048);
	load_ssf         = (void*)(addr + 0x10FB08);
	load_script_data = (void*)(addr + 0x10E438);
	create_plr       = (void*)(addr + 0x153960);
	string_right_xy  = (void*)(addr + 0x796A4);
	string_left_xy   = (void*)(addr + 0x79624);
	func_6F388       = (void*)(addr + 0x6F388);
}

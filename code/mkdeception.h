#pragma once
#include "mkstructs.h"
#include "ssf.h"

enum PLAYER_NUM
{
	INVALID_PLAYER_NUM = -1,
	PLAYER1,
	PLAYER2,
};

#define PLAYER1_ADDR  0x58E9AC
#define PLAYER2_ADDR  0x58EA18

// game functions
extern void(*mk_sprintf)(char* ptr, char* fmt, ...);
extern void(*load_ssf)(struct mem_ssf_header* ssf);
extern int (*load_script_data)(int id, char* name);
extern int (*create_plr)(int plr, struct player_info* obj);
extern int (*string_right_xy)(int id, int font, char* text, int y, int x, int unk);
extern int (*string_left_xy)(int id, int font, char* text, int y, int x, int unk);
extern void(*func_6F388)(int unk, int id);

void MKDeception_Init(unsigned int addr);

// hooks
int  hook_create_player(int plr, struct player_info* obj);

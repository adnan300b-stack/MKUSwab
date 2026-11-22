#include <pspsdk.h>
#include <pspkernel.h>
#include <stdio.h>
#include <systemctrl.h>
#include <string.h>

#include "code/config.h"
#include "code/pspmem.h"
#include "code/mkdeception.h"
#include "code/ssf.h"
#include "code/eLog.h"
#include "code/eSwapMgr.h"
#include "code/mkuplrinfo.h"

PSP_MODULE_INFO(MODULE_NAME, 0, 1, 0);
static STMOD_HANDLER previous;

int Init(u32 addr)
{
	LOG_Initialise();
	PSPPatcher_Init(addr);
	MKDeception_Init(addr);
	calculate_custom_toc();
	SwapMgr_Init();

	// player creation (tracks which player is being loaded)
	MakeCall(0x70990,  (int)custom_create_player);
	MakeCall(0xBCE4C,  (int)custom_create_player);
	MakeCall(0x14EFAC, (int)custom_create_player);
	MakeCall(0x14EFC0, (int)custom_create_player);

	// model/anims loading (these get swapped)
	MakeCall(0x153B00, (int)custom_load_ssf_alt);
	MakeCall(0x153B1C, (int)custom_load_ssf);
	MakeCall(0x1506A8, (int)custom_load_ssf_async);
	MakeCall(0x150730, (int)custom_load_ssf_async);

	// script loading (specials - intentionally NOT swapped)
	MakeCall(0x150424, (int)custom_load_script_data);
	MakeCall(0x1506B4, (int)custom_load_script_data_async);

	// select screen helper loads
	MakeCall(0x6F584, (int)unk_load_ssf);
	MakeCall(0x6F3E8, (int)unk_load_ssf);
	MakeCall(0x6F3F4, (int)unk_load_script_data);
	MakeCall(0x6F2A8, (int)hook_unk_func);
	MakeCall(0x6F2CC, (int)hook_unk_func);
	MakeCall(0x6F6A8, (int)hook_unk_func);

	// HUD names show the visual character
	MakeCall(0x16824C, (int)player1_name);
	MakeCall(0x16836C, (int)player2_name);
	MakeCall(0x82B04,  (int)set_win_name);

	LOG_Message("INFO: all hooks installed\n");
	return 0;
}

int OnModuleStart(SceModule2* mod)
{
	char* modname = mod->modname;

	if (strcmp(modname, TARGET_GAME) == 0) {
		int ret = Init(mod->text_addr);
		if (ret != 0)
			return -1;

		sceKernelDcacheWritebackAll();
		sceKernelIcacheClearAll();
	}
	if (!previous)
		return 0;

	return previous(mod);
}

int module_start(SceSize argc, void* argp)
{
	if (sceIoDevctl("kemulator:", 0x00000003, NULL, 0, NULL, 0) == 0) {
		SceUID modules[10];
		int count = 0;
		if (sceKernelGetModuleIdList(modules, sizeof(modules), &count) >= 0) {
			int i;
			SceKernelModuleInfo info;
			for (i = 0; i < count; ++i) {
				info.size = sizeof(SceKernelModuleInfo);
				if (sceKernelQueryModuleInfo(modules[i], &info) < 0) {
					continue;
				}
				if (strcmp(info.name, TARGET_GAME) == 0) {
					int ret = Init(info.text_addr);
					if (ret != 0)
						return -1;

					sceKernelDcacheWritebackAll();
					sceKernelIcacheClearAll();
					return 0;
				}
			}
		}
	}
	else {
		previous = sctrlHENSetStartModuleHandler(OnModuleStart);
	}

	return 0;
}

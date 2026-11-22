#pragma once

#define MAX_SWAPS 32

// One entry of "chang(VISUAL->SELECTED)":
// when the user picks character "selected", the game loads
// the model/anims of character "visual" instead.
struct char_swap {
	int  selected_id; // character the user picks   (right side)
	int  visual_id;   // character that appears     (left side)
	char raw[96];     // original line, kept for logging
};

int SwapMgr_Init();
int SwapMgr_GetVisual(int selected_id);
int SwapMgr_Count();

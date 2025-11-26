// キー入力管理
#pragma once
#include "WinMain.h"

void KeyInputInit();			// 初期化
int KeyInputUpdate();			// 更新
void KeyInputExit();			// 終了


bool GetKeyPush(int key_id);		// 1回だけ
bool GetKeyDown(int key_id);		// 1回だけ
bool GetKeyRelease(int key_id);	// 離された
bool GetKeyRepeat(int key_id);	// 2回以上(長押し)

bool GetMouseInput(int mouse_id);

bool IsAdvanceCutsceneButton();
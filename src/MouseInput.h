// マウス入力管理
#pragma once
#include "WinMain.h"

void MouseInputInit();		// 初期化
void MouseInputUpdate();	// 更新
void MouseInputExit();		// 終了

bool IsMouseOn(int mouse_id);			// 1回だけ
bool IsMouseRelease(int mouse_id);		// 離された
bool IsMouseRepeat(int mouse_id);		// 長押し

int GetMouseX();			// マウスのX座標 取得
int GetMouseY();			// マウスのY座標 取得

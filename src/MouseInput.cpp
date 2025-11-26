// マウス入力管理
#include "MouseInput.h"

#define MAX_MOUSE_BUTTON	8

static int mouse_x;
static int mouse_y;

static int gMouse[MAX_MOUSE_BUTTON];

static bool IsOverMouseNum(int mouse_id);


// 初期化
void MouseInputInit()
{
	mouse_x = 0;
	mouse_y = 0;

	for (int i = 0; i < MAX_MOUSE_BUTTON; ++i) {
		gMouse[i] = 0;
	}
}

// 更新
void MouseInputUpdate()
{
	int mouse_state = GetMouseInput();
	GetMousePoint(&mouse_x, &mouse_y);

	for (int i = 0; i < MAX_MOUSE_BUTTON; ++i) {
		if (mouse_state - 1 == i) {
			++gMouse[i];
			if (gMouse[i] >= INT_MAX)	gMouse[i] = INT_MAX;
		}
		else {
			gMouse[i] = 0;
		}
	}
}

// 終了
void MouseInputExit()
{
	;
}

// 1回だけ
bool IsMouseOn(int mouse_id)
{
	int tmp_mouse_id = mouse_id - 1;
	if (IsOverMouseNum(tmp_mouse_id))	return false;
	return (gMouse[tmp_mouse_id] == 1);
}

// 離された
bool IsMouseRelease(int mouse_id)
{
	int tmp_mouse_id = mouse_id - 1;
	if (IsOverMouseNum(tmp_mouse_id))	return false;
	return (gMouse[tmp_mouse_id] == 0);

}

// 長押し
bool IsMouseRepeat(int mouse_id)
{
	int tmp_mouse_id = mouse_id - 1;
	if (IsOverMouseNum(tmp_mouse_id))	return false;
	return (gMouse[tmp_mouse_id] > 1);
}

// マウスのX座標 取得
int GetMouseX()
{
	return mouse_x;
}

// マウスのY座標 習得
int GetMouseY()
{
	return mouse_y;
}

// マウスの配列検証用
bool IsOverMouseNum(int mouse_id)
{
	return (mouse_id >= MAX_MOUSE_BUTTON) || (mouse_id < 0);
}
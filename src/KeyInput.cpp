// キー入力管理
#include "KeyInput.h"

#define MAX_KEY_NUM 256

static unsigned char gKey[MAX_KEY_NUM];

static bool IsOverKeyNum(int key_id);	// 配列の検証用

// 初期化
void KeyInputInit()
{
	for (int i = 0; i < MAX_KEY_NUM; ++i) {
		gKey[i] = 0;
	}
}
// 更新
int KeyInputUpdate()
{
	char tmp_key[MAX_KEY_NUM];
	GetHitKeyStateAll(tmp_key);

	for (int i = 0; i < MAX_KEY_NUM; ++i) {
		if (tmp_key[i] != 0) {
			gKey[i]++;
			if (gKey[i] >= UCHAR_MAX)	gKey[i] = UCHAR_MAX;
		}
		else {
			gKey[i] = 0;
		}
	}
	return 0;
}
// 終了
void KeyInputExit()
{
	;
}

// 特定キーの1回だけ押下検証
bool GetKeyPush(int key_id)
{
	if (IsOverKeyNum(key_id)) return false;
	return (gKey[key_id] == 1);
}
bool GetKeyDown(int key_id)
{
	if (IsOverKeyNum(key_id)) return false;
	return (gKey[key_id] == 1);
}

// 特定キーの解放検証
bool GetKeyRelease(int key_id)
{
	if (IsOverKeyNum(key_id)) return false;
	return (gKey[key_id] == 0);
}
// 特定キーの長押し検証
bool GetKeyRepeat(int key_id)
{
	if (IsOverKeyNum(key_id)) return false;
	return (gKey[key_id] > 1);
}
// gKey配列検証用
bool IsOverKeyNum(int key_id)
{
	return (key_id >= MAX_KEY_NUM) || (key_id < 0);
}

bool GetMouseInput(int mouse_id)
{
	return GetMouseInput() && mouse_id != 0;
}

bool IsAdvanceCutsceneButton()
{
	return (GetKeyDown(KEY_INPUT_RETURN) || GetKeyDown(KEY_INPUT_SPACE) || GetMouseInput(INPUT_MOUSE_LEFT));
}

#include "WinMain.h"
#include "../customsrc/GameTimer.h"
#include "../customsrc/PauseMenu.h"

// Function to confine the mouse within the window
void ConfineMouseWithinWindow(HWND gamewindowhandler)
{
	RECT windowRect;
	GetWindowRect(gamewindowhandler, &windowRect); // Get the window's rectangle

	// Clip the cursor within the window boundaries
	ClipCursor(&windowRect);
}

void ReleaseMouseConfine()
{
	ClipCursor(NULL); // Release the mouse confinement
}

extern Time time;
extern PauseMenu pausemenu;
bool isGamePaused = false;
bool isWindowedMode = true;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetOutApplicationLogValidFlag(FALSE);
	ChangeWindowMode(isWindowedMode);
	SetGraphMode(WINDOW_W, WINDOW_H, 32);
	SetBackgroundColor(0,0,0);
	SetMainWindowText("The tunnel...............");
	if (DxLib_Init() == -1) { return -1; }
	SetDrawScreen(DX_SCREEN_BACK);
	// ---------------
	// èâä˙âªèàóù
	// ---------------
	KeyInputInit();
	PadInputInit();
	MouseInputInit();
	GameInit();

	HWND gameWindowHandler;
	gameWindowHandler = GetMainWindowHandle();
	//SetWaitVSyncFlag(true);
	while (ProcessMessage() == 0)
	{

		if (gameWindowHandler != GetForegroundWindow())
		{
			pausemenu.SetPauseGame();
		}
		if (!isGamePaused)
		{
		ConfineMouseWithinWindow(gameWindowHandler);
		}
		if (isGamePaused)
		{
			ReleaseMouseConfine();
		}
		clsDx();
		// ---------------
		// çXêVèàóù
		// ---------------
		KeyInputUpdate();
		PadInputUpdate();
		MouseInputUpdate();
		GameUpdate();

		// ---------------
		// ï`âÊèàóù
		// ---------------
		ClearDrawScreen();
		GameDraw();
		ScreenFlip();
	}

	// ---------------
	// èIóπèàóù
	// ---------------
	KeyInputExit();
	PadInputExit();
	MouseInputExit();
	GameExit();

	DxLib_End();

	return 0;
}

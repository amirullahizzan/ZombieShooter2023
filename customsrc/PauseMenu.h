#pragma once
#include "../src/WinMain.h"
#include "Loader.h"
#include "Pos.h"
#include "SceneManager.h"
#include "Hitbox.h"

extern Loader loader;
extern bool isGamePaused;
extern int currentScene;

class PauseMenu
{
public :
	PauseMenu() : pos1(0, 0), pos2(0, 0), MAXSIZE1(0, 0), MAXSIZE2(WINDOW_W, WINDOW_H) {}
	PauseMenu(const Float2& _xy1, const Float2& _xy2, const Float2& _MAXSIZE1, const Float2& _MAXSIZE2)
		: pos1(_xy1), pos2(_xy2), MAXSIZE1(_MAXSIZE1), MAXSIZE2(_MAXSIZE2) {}
private : 
	int animMode = NoAnim;
	enum PauseMenuEnum
	{
		SettingsMenu = 0,
		ExitGame = 1,
	};

	enum AnimModeEnum
	{
		Open = 0,
		Close = 1,
		NoAnim = 2
	};
	
	void AnimateVoidBox()
	{
	
		const int sizemodifier = 1200;

		switch (animMode)
		{
		case Open:
				isOpen[SettingsMenu] = true;
				if (pos1.x > MAXSIZE1.x)
				{
					pos1.x-= sizemodifier * time.deltaTime;
				}
				if (pos2.x < MAXSIZE2.x)
				{
					pos2.x+= sizemodifier * time.deltaTime;
				}
				if (pos1.x < MAXSIZE1.x && pos2.x > MAXSIZE2.x) { animMode = NoAnim; }
					
			break;
		case Close:
			if (pos1.x < WINDOW_W_HALF)
			{
				pos1.x+= sizemodifier * time.deltaTime;
			}
			if (pos2.x > WINDOW_W_HALF)
			{
				pos2.x-= sizemodifier * time.deltaTime;
			}
			if (pos1.x >= WINDOW_W_HALF+1 || pos2.x <= WINDOW_W_HALF-1)
			{
				isOpen[SettingsMenu] = false;
				animMode = NoAnim;
			}
			break;
		case NoAnim :
			break;
		}
	}

public :
	bool isPauseAllowed = false;

	void PauseMenuUpdate()
	{
		
	}

	Float2 pos1;
	Float2 pos2;

	Float2 MAXSIZE1;
	Float2 MAXSIZE2;

	Float2 mousepos_;

	Button closeButton = { {pos1.x-20,pos1.y - 20},20,20 };
	static const int MAX_MENU = 2;



	void SetPauseGame()
	{
		if (!isGamePaused && isPauseAllowed)
		{
		isGamePaused = true;
		}
	}

	void UpdatePause()
	{
		if(isGamePaused)
		{ 
		}
	}

	bool isGameTutorial = false;

	void RenderPause()
	{
		if (isGamePaused && !isGameTutorial)
		{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,220);
		DrawBoxAA(0, 0, WINDOW_W, WINDOW_H,BLACK,1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
		SetFontSize(FONTSIZE_LARGE);
		DrawString(WINDOW_W_HALF-165, WINDOW_H_HALF-25,"Game is paused.",GRAY);
		SetFontSize(FONTSIZE_NORMAL);
		}
	}

	void RenderSetting()
	{
		AnimateVoidBox();
		if (isOpen[SettingsMenu])
		{
			DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA,240);
			DrawBoxAA(pos1.x, pos1.y, pos2.x, pos2.y, DARKERGRAY, 1);
			DrawLineBox(static_cast<int>(pos1.x), static_cast<int>(pos1.y), static_cast<int>(pos2.x), static_cast<int>(pos2.y), RED);
			DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
			
			{
				DrawGraphF(closeButton.pos1.x, closeButton.pos1.y, loader.icon_close_handler[0], 1);
				DrawGraphF(closeButton.pos1.x, closeButton.pos1.y, loader.icon_close_handler[1], 1);
			}
		}
		
	}

	void OpenSetting()
	{
		animMode = Open;
	}

	void CloseSetting()
	{
		animMode = Close;
	}

	bool IsInMenu()
	{
		return isOpen[SettingsMenu];
	}

	void UpdateSetting(const Float2& mousepos, const float mouseradius)
	{
		if (animMode == NoAnim)
		{
			bool mouseLeft = GetMouseInput(MOUSE_INPUT_LEFT);
			bool escapeKey = GetKeyDown(KEY_INPUT_ESCAPE);
			bool mouseHitbutton = HitboxPointBox(mousepos, closeButton, MOUSE_RADIUS);
			if (	mouseHitbutton && mouseLeft || escapeKey )
			{
				CloseSetting();
			}
		}
	}


private :
	bool isOpen[MAX_MENU] = {false,false};
};
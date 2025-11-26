#pragma once
#include "../src/WinMain.h"

//######################
struct DebugTools
{
public:
	
	void DrawXGrid()
	{
	DrawLine(0, WINDOW_H_HALF, WINDOW_W, WINDOW_H_HALF, YELLOW, 1);
	}
	void DrawYGrid()
	{
	DrawLine(WINDOW_W_HALF,0, WINDOW_W_HALF, WINDOW_H,YELLOW, 1);
	}

	void DrawQuickBox(float x = WINDOW_W/2, float y = WINDOW_H / 2, float width = 200, float height = 200)
	{
		DrawBoxAA(x,y,x+width, y + height,WHITE,1,1);
	}

};
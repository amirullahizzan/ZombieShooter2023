#pragma once
#include "../src/WinMain.h"
#include "Pos.h"

//move float2 to pos

struct Box
{
public :
	Box() : pos1(0, 0), pos2(0, 0), width(0), height(0) {}
	//Set param requirements : finalize param, bring it inside this class' member values
	Box(const Float2& _pos1, float _width, float _height)
		: pos1(_pos1), width(_width), height(_height)
	{
		pos2.x = pos1.x + _width;
		pos2.y = pos1.y + _height;
	}
	Box(const Float2& _pos1, const Float2& _pos2)
		: pos1(_pos1), pos2(_pos2)
	{
		width = pos1.x + pos2.x;
		height = pos1.x + pos2.x;
	}

	Float2 pos1;
	Float2 pos2;

	float width = 0;
	float height = 0;
	int color_red = 255;
	int color_green = 255;
	int color_blue = 255;

	void SetPos1Whole()
	{
		pos2.x = pos1.x + width;
		pos2.y = pos1.y + height;
	}

	void SetPos1Whole(float _width,float _height)
	{
		pos2.x = pos1.x + _width;
		pos2.y = pos1.y + _height;
	}
};

struct Circle
{
	Float2 pos1;
	float radius;
	Circle() : pos1(0,0), radius(0) {};
	Circle(const Float2& pos1 ,float _rad) : pos1(0,0), radius(_rad) {};

};

class FadeBox : public Box
{
public:
	enum FadeModeEnum
	{
		NoMode = 0,
		FadeOutMode = 1, //TO NO BLACK
		FadeInMode = 2, //TO FULL BLACK
	};
	FadeBox(FadeModeEnum _fadeMode) : fadeMode(_fadeMode)
	{	}

private :

	int fadeMode = 0;
	int blendparam = 255;
	int blendmodifier = 1;


	void FadeOut()
	{
		if (blendparam <= DX_MAX_ALPHA)
		{
			blendparam += blendmodifier;
		}
		if (blendparam >= DX_MAX_ALPHA)
		{
			fadeMode = NoMode;
		}
	}

	void FadeIn()
	{
		if (blendparam >= DX_NO_ALPHA)
		{
			blendparam -= blendmodifier;
		}
		if (blendparam <= DX_NO_ALPHA)
		{
			fadeMode = NoMode;
		}
	}

public :

	void Update()
	{
		switch (fadeMode)
		{
		case FadeOutMode:
			FadeOut();
			break;
		case FadeInMode:
			FadeIn();
			break;
		case NoMode:
			break;
		}
	}

	void RenderFade()
	{
		DxLib::SetDrawBlendMode(DX_BLENDMODE_ALPHA,blendparam);
		DrawBoxAA(0,0,WINDOW_W, WINDOW_H,BLACK,1,1);
		DxLib::SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

	void SetFadeOut(int _blendmodifier = 1)
	{
		blendmodifier  = _blendmodifier;
		fadeMode = FadeOutMode;
	}

	void SetFadeIn(int _blendmodifier = 1)
	{
		blendmodifier = _blendmodifier;
		fadeMode = FadeInMode;
	}

	bool IsFadeOut()
	{
		if (fadeMode == FadeOutMode)
		{
			return true;
		}
		return false;
	}

	bool IsFadeIn()
	{
		if (fadeMode == FadeInMode)
		{
			return true;
		}
		return false;
	}

	bool IsNoMode()
	{
		if (fadeMode == NoMode)
		{
			return true;
		}
		return false;
	}

	int GetFadeMode()
	{
		return fadeMode;
	}

};
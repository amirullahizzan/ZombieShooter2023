#pragma once
#include "Primitive.h"
#include "Loader.h"
#include <string>

extern int dialogueIndex;
extern int currentScene;

extern Loader loader;

int GetSceneMode();
void SetSceneMode(int sceneIndex);

class Button : public Box
{
	const int LOWEST_COLOR = 127;
	int color_red = LOWEST_COLOR;
	int color_green = LOWEST_COLOR;
	int color_blue = LOWEST_COLOR;


	bool isAnimated = false;
	int blendparam = 0;
	const int BASE_blendparam = 0;

	void SelectorAnim()
	{
		constexpr int blendparam_modifier = 3;
		static bool isDim = true;

		constexpr int animtick = 1;
		static int animframe = 0;
		animframe++;
		
			if (animframe % animtick == 0 && blendparam <= DX_HALF_ALPHA)
			{
				blendparam += blendparam_modifier;
				animframe = 0;
			}
	}
public :
	Button(Float2 boxpos,float _width, float _height) : Box(boxpos, _width, _height)
	{
		pos1.x = pos1.x-width;
	}

	void SetIsAnimTrue()
	{
		isAnimated = true;
	}
	void SetIsAnimFalse()
	{
		isAnimated = false;
	}

	void RenderSelection(int imagehandler)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendparam);
		DrawExtendGraphF(pos1.x, pos1.y-50, pos2.x, pos2.y+50, imagehandler, 1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

		if (isAnimated)
		{
			SelectorAnim();
			HighlightText();
		}
		else
		{
			blendparam = BASE_blendparam;
			SetTextColor(LOWEST_COLOR, LOWEST_COLOR, LOWEST_COLOR);
		}
	}

	int GetTextColor()
	{
		return GetColor(color_red, color_green, color_blue);
	}

	void SetTextColor(int red, int green, int blue)
	{
		color_red = red, color_green = green, color_blue = blue;
	}

	void HighlightText()
	{
		static bool iscolorGrey = false;

		if (color_red < LOWEST_COLOR)
		{
			iscolorGrey = true;
		}
		else if(color_red >= 254)
		{
			iscolorGrey = false;
		}

		const int colormodifier = 2;
		if (iscolorGrey)
		{
			//TURN WHITE
			color_red += colormodifier;
			color_green += colormodifier;
			color_blue += colormodifier;
		}
		else
		{
			//TURN GRAY
			color_red -= colormodifier;
			color_green -= colormodifier;
			color_blue -= colormodifier;
		}

	}

};

class UIManager
{
private:	
	float selectorcursor_y = 0;
	float* currentSelectorCursor_y = &selectorcursor_y;

public:
	float* GetSelectorCursorPosY()
	{
		return currentSelectorCursor_y;
	}

	void SetSelectorCursorPosY(float &selectorposy)
	{
		const float addposy = 70;
		*currentSelectorCursor_y = selectorposy + addposy;
		//currentSelectorCursor_y = &selectorposy;
	}
	

	int font_handler = 0;

	void RenderCursor(float x_new, float* y_new)
	{
		static float x = 0;
		static float y = 0 - 30;

		//######################################
		//Up down Anim
		{
		static bool isMovingRight = false;
			static int animindex = 0;
			static float animframe = 0;
			animframe+=time.deltaTime;

			constexpr float animtimer = 0.7f;
			if (animframe > animtimer)
			{
				isMovingRight = !isMovingRight;
				animframe = 0;
			}
			{
				switch (isMovingRight)
				{
				case false:
					x -= 0.3f;
					break;
				case true:
					x += 0.3f;
					break;
				}
			}
		}
		//######################
		//Cursor Anim
		{

			static int animindex = 0;
			static float animframe = 0;
			animframe += time.deltaTime;
			DrawRotaGraphF(x + x_new, y + *y_new, 3, 0, loader.icon_cursor_handler[animindex], 1);

			constexpr float animtimer = 0.2f;
			if (animframe > animtimer)
			{
				animindex++;
				animframe = 0;
			}

			if (animindex >= loader.icon_cursor_handler_ANIMS)
			{
				animframe = 0;
				animindex = 0;
			}
		}
		//######################################
	}
};

struct DialogueData
{
public :
	DialogueData(TCHAR _dialogue) : dialogue(_dialogue) {}

	TCHAR dialogue
	{

	};
};

class DialogueSystem
{
	Float2 pos_;

	const char* DialogueList[3] =
	{
		{"...."},
		{"?"},
		{"?"}
	};

	void NextDialogue()
	{
		dialogueIndex++;
		//if press enter
	}

public:

	DialogueSystem()
	{

	}


void DisplayDialogue()
{
	
}


};


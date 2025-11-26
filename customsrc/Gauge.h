#pragma once
#include "Primitive.h"

class Bar
{
public:
	Bar(Float2 _pos1, float _width, float _height) : box(_pos1,_width,_height){};
	Box box;
	enum FACING
	{
		RIGHT, DOWN, LEFT, UP
	};
	void RenderHorizontal(float currentvalue, const float maxvalue, FACING facing, int _color)
	{
		static float barWidth = 0;

		if (facing == FACING::RIGHT)
		{
			barWidth = (currentvalue * box.width) / maxvalue;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos2.x, box.pos2.y, GetColor(255, 255, 255), 1);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos1.x + barWidth, box.pos2.y, _color, 1);
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos2.x, box.pos2.y, GetColor(125, 125, 125),0);
		}
		else if (facing == FACING::LEFT)
		{
			barWidth = (currentvalue * box.width * -1) / maxvalue;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos1.x - box.width, box.pos2.y, GetColor(255, 255, 255), 1);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos1.x + barWidth, box.pos2.y, _color, 1);
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos1.x - box.width, box.pos2.y, GetColor(125, 125, 125),0);
		}
	}

	void RenderVertical(float currentvalue, const float maxvalue, FACING facing, int _color)
	{
		float barHeight = 0;

		if (facing == FACING::DOWN)
		{
			float barHeight = (currentvalue * box.height) / maxvalue;
			//Frame
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos2.x, box.pos2.y, GetColor(255, 255, 255), 1);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			//MainBox
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos2.x, box.pos1.y + barHeight, _color, 1);
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos2.x, box.pos2.y, GetColor(125, 125, 125),0);
		}
		else if (facing == FACING::UP)
		{
			float barHeight = (currentvalue * box.height * -1) / maxvalue;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos2.x, box.pos1.y - box.height, GetColor(255, 255, 255), 1);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos2.x, box.pos1.y + barHeight, _color, 1);
			DrawBoxAA(box.pos1.x, box.pos1.y, box.pos2.x, box.pos1.y - box.height, GetColor(125, 125, 125),0);
		}

	}

};

//Bar barright = { {WINDOW_W / 2,WINDOW_H / 2,200,40} };
//Bar bardown = { {WINDOW_W / 2 - 60,WINDOW_H / 2,40,200} };
//Bar barleft = { {WINDOW_W / 2,WINDOW_H / 2 - 60,200,40} };
//Bar barup( {WINDOW_W / 2 - 180,WINDOW_H / 2} , 40, 200);

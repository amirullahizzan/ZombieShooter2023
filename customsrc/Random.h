#pragma once
#include "../src/WinMain.h"

float GetRandomF()
{
	float random = (float)rand();
	return random / RAND_MAX;
}
//---------------------------------------------------------------------------------
//	0.0 ～ f_max までのランダムな値を返す関数（ float 型）
//---------------------------------------------------------------------------------
float GetRandomF(float f_max)
{
	float random = GetRandomF();
	return random * f_max;
}
//---------------------------------------------------------------------------------
//	f_from ～ f_to までのランダムな値を返す関数（ float 型）
//---------------------------------------------------------------------------------
float GetRandomF(float f_from, float f_to)
{
	float value = f_to - f_from;
	float random = GetRandomF(value);
	return f_from + random;
}
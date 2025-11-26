#pragma once

#include "DxLib.h"
#include "KeyInput.h"
#include "PadInput.h"
#include "MouseInput.h"
#include "GameMain.h"
#include <math.h>
#include <string>
#include <memory>
#include <vector>
#include <algorithm>

#define PI		 3.141592653589793
//PI is 180 degrees
#define RadToDeg 57.29577951f
#define R2D(rad) rad * RadToDeg

#define DegToRad 0.017453293f
#define D2R(deg) deg * DegToRad

#define KEY_INPUT_ENTER	  KEY_INPUT_RETURN
#define INPUT_MOUSE_LEFT  MOUSE_INPUT_LEFT
#define INPUT_MOUSE_RIGHT MOUSE_INPUT_RIGHT

#define BLACK	      GetColor(0, 0, 0)
#define WHITE	      GetColor(255, 255, 255)
#define RED		      GetColor(255, 0, 0)
#define GREEN	      GetColor(0, 255, 0)
#define BLUE	      GetColor(0, 0, 255)
#define YELLOW	      GetColor(255, 255, 0)
#define DARKYELLOW	  GetColor(220, 220, 0)
#define PINKPASTEL    GetColor(255, 192, 203)
#define PINK	      GetColor(238, 130, 203)
#define PURPLEMAGENTA GetColor(255, 0, 255)
#define PURPLE	      GetColor(160, 32, 240)
#define DARKPURPLE	  GetColor(128, 0, 128)
#define SKYBLUE	      GetColor(0, 255, 255)
#define LIGHTGRAY     GetColor(170, 170, 170)
#define GRAY	      GetColor(127, 127, 127)
#define DARKGRAY      GetColor(63, 63, 63)
#define DARKERGRAY    GetColor(31, 31, 31)
#define BROWN	      GetColor(101, 67, 33)

#define WINDOW_W	1280
#define WINDOW_H	720

int constexpr WINDOW_W_HALF = WINDOW_W / 2;
int constexpr WINDOW_H_HALF = WINDOW_H / 2;
int constexpr WINDOW_H_DIV3 = WINDOW_H / 3;


//#################################
constexpr int SIDEBAR_SIZE = 200;
constexpr int WINDOW_W_INNERLEFT = 0 + SIDEBAR_SIZE;
constexpr int WINDOW_W_INNERRIGHT = WINDOW_W - SIDEBAR_SIZE;
constexpr int WINDOW_W_INNER_WIDTH = WINDOW_W_INNERRIGHT - WINDOW_W_INNERLEFT;

#define DX_MAX_ALPHA 255
#define DX_HALF_ALPHA 127
#define DX_NO_ALPHA 0

#define FONTSIZE_TITLE 80
#define FONTSIZE_LARGE 50
#define FONTSIZE_NORMAL 30
#define FONTSIZE_SMALL 25

constexpr int MOUSE_RADIUS = 15;
const int MAX_BULLET_i = 50;
const int MAX_ENEMY_i = 35;

namespace customlib {

	template<typename T>
	constexpr const T& clamp(const T& value, const T& min, const T& max) {
		return (value < min) ? min : ((value > max) ? max : value);
	}

}

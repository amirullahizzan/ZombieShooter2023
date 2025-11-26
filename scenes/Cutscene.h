#pragma once

class TalkBox
{
private:
	Float2 boxCenter = { WINDOW_W / 2, WINDOW_H - 160};
	Box box = { { boxCenter.x , boxCenter.y }, -1 ,180};
	float counter_ms = 0;
	bool isBoxAnimDone = false;

	float x_anim = 0;
	float y_anim = 0;

	int cursor_y_anim = 0;
	int cursor_y_anim_ms = 0;

protected:
	void DrawContinueIcon(float _x, float _y)
	{
		static int icon_anim_i = 0;
		static Timer animtimer;
		animtimer.Update();
		animtimer.SimpleAnimIndex(icon_anim_i, loader.icon_cursor_handler_ANIMS, 0.2f);
		DrawRotaGraphF(_x, _y + cursor_y_anim, 3, PI * 0.5f, loader.icon_cursor_handler[icon_anim_i], 1, 0, 0);
		cursor_y_anim_ms++;
		if (cursor_y_anim_ms > 6)
		{
			cursor_y_anim++;
			cursor_y_anim_ms = 0;
			if (cursor_y_anim > 7)
			{
				cursor_y_anim = 0;
			}
		}
	}

	void RenderCloseTalkBox(float _width)
	{
		x_anim += time.deltaTime *(_width * 1.8f);
		y_anim += time.deltaTime * (box.height * 1.8f);
		DrawBoxAA(boxCenter.x - x_anim, boxCenter.y - y_anim, boxCenter.x + x_anim, boxCenter.y + y_anim, BLACK, 1);
		//DrawBoxAA(box.pos1.x , box.pos1.y , box.pos2.x , box.pos2.y , WHITE, 1);

		if (counter_ms > 0.6f)
		{
			isBoxAnimDone = true;
		}
	}

	Box unboxing;
	bool isUnboxingExist = false;
	bool isUnboxingDone = false;
	void RenderUnboxingBox()
	{
		if (!isUnboxingExist)
		{
		unboxing = box;
		isUnboxingExist = true;
		}

		if (unboxing.pos1.x < unboxing.pos2.x)
		{
		unboxing.pos1.x += 8;
		isUnboxingDone = true;
		}
		DrawBoxAA(unboxing.pos1.x, unboxing.pos1.y, unboxing.pos2.x, unboxing.pos2.y, BLACK, 1);
	}

public:
	TalkBox(float _width, const char* _string, const char* _namestring = "") : string_(_string) , namestring_(_namestring)
	{
		box.width = _width;
		bool isDone = false;
	}
	const char* string_ = nullptr;
	const char* namestring_ = nullptr;
	bool isDone = false;

	void RenderDialogBox()
	{
		counter_ms+=time.deltaTime*2;
		//DEBUG
		//DrawCircleAA(boxCenter.x, boxCenter.y,25,64,WHITE,1);

		if (!isBoxAnimDone)
		{
			RenderCloseTalkBox(box.width);
		}
		if (isBoxAnimDone)
		{
			if (!isDone)
			{
				box.pos1.x = boxCenter.x - box.width / 2;
				box.pos1.y = boxCenter.y - box.height / 2;
				box.pos2.x = boxCenter.x + box.width / 2;
				box.pos2.y = boxCenter.y + box.height / 2;

				//BORDER BOX
				int border = 3;
				DrawBoxAA(box.pos1.x - border, box.pos1.y - border, 
						  box.pos2.x + border, box.pos2.y + border, WHITE, 1);
				//INNER BOX
				DrawBoxAA(box.pos1.x , box.pos1.y, box.pos2.x, box.pos2.y, BLACK, 1);

				//NAME
				SetFontSize(FONTSIZE_SMALL);
				Float2 nameindent = { 25.0f,15.0f };
				DrawStringF(box.pos1.x + nameindent.x, box.pos1.y + border + nameindent.y, namestring_, YELLOW, 0);

				//LINE
				Float2 lineposindent = {25.0f,40.0f} ;
				Float2 linepos1 = { box.pos1.x + lineposindent.x,box.pos1.y + lineposindent.y} ;
				float linelength = 50;
				Float2 linepos2 = { linepos1.x + linelength, linepos1.y} ;
				DrawLineAA(linepos1.x , linepos1.y ,
						   linepos2.x, linepos2.y, PURPLEMAGENTA, 5);

				//STRING
				Float2 stringindent = { 25.0f,lineposindent.y + 10.0f};
				DrawStringF(box.pos1.x + stringindent.x, box.pos1.y + border + stringindent.y, string_, WHITE, 0);
				SetFontSize(FONTSIZE_NORMAL);

				RenderUnboxingBox();
				//ICON
				Float2 iconpos;
				iconpos.x = box.pos2.x - 20;
				iconpos.y = box.pos2.y - 80;
				DrawContinueIcon(iconpos.x, iconpos.y);

			}
			if ( (GetKeyDown(KEY_INPUT_RETURN) || GetKeyDown(KEY_INPUT_SPACE)  || GetMouseInput(INPUT_MOUSE_LEFT) ) && isBoxAnimDone && isUnboxingDone)
			{
				isDone = true;
			}
		}

	}

};



void InitCutscene()
{
	StopSoundMem(loader.terror_bgm_handler);
	StopSoundMem(loader.scary1_bgm_handler);
	isCutsceneOver = false;
	fadebox_ptr->SetFadeIn(1);
	switch (cutsceneMode)
	{
	case CutsceneIntro:
		break;

	case CutsceneGameEnding:
		break;
	}
}



void UpdateCutscene()
{
	switch (cutsceneMode)
	{
	case CutsceneIntro:
		if (isCutsceneOver)
		{
			SetSceneMode(InitGameplayScene);
		}
		break;
	case CutsceneGameEnding:
		if (isCutsceneOver)
		{
			SetSceneMode(InitMainMenuScene);
		}
		break;
	}
}

class DramaticPicture
{
private:

public:
	enum ZoomEffect
	{
		ZoomIn = 0,
		ZoomOut = 1
	};

	DramaticPicture(int _image_handler, ZoomEffect _effect)
	{
	image_handler_ = _image_handler;
	effect_ = _effect;
	}

	int image_handler_ = 0;
	int effect_ = 0;

void RenderFadeInPicture()
{
	Float2 pos = {WINDOW_W_HALF,WINDOW_H_HALF };
	double imagesize = -1;
	switch (effect_)
	{
	case ZoomIn:
		if (imagesize < 1) { imagesize = 1; }
		if (imagesize < 2) 
		{
		imagesize += time.deltaTime;
		}
		break;

	case ZoomOut:
		if (imagesize < 1) { imagesize = 2; }
		if (imagesize > 1 )
		{
		imagesize -= time.deltaTime;
		}
		break;
	}
	//SETDRAWBLEND
	DrawRotaGraphF(pos.x, pos.y, imagesize , 0, image_handler_,0);
}

};

TCHAR creditslist[]
{
	("muzzle_flash : Pngtree.com\n Main menu music and SE : pixabay.com")
};

void RenderCutscene()
{
	DrawBoxAA(0, 0, WINDOW_W,WINDOW_H, DARKERGRAY, 1);
	switch (cutsceneMode)
	{
	case CutsceneIntro:
		if (!fadebox_ptr->IsFadeIn())
		{
		static std::unique_ptr<TalkBox> tbox1 = std::make_unique<TalkBox>(600.0f,"My name is John", "John");
		tbox1->RenderDialogBox();
		if (tbox1->isDone)
		{
			static std::unique_ptr<TalkBox> tbox2 = std::make_unique<TalkBox>(800.0f, "and I have to save my daughter.", "John");
			tbox2->RenderDialogBox();
			if (tbox2->isDone)
			{
				static std::unique_ptr<DramaticPicture> pic1 = std::make_unique<DramaticPicture>(loader.introcutscene1_handler, DramaticPicture::ZoomOut);
				pic1->RenderFadeInPicture();
				//IF PRESS ENTER, CONTINUE DIALOGUE, ON CERTAIN TIME CONTINUE IMAGE
				static std::shared_ptr<Timer> newtimer2 = std::make_shared<Timer>();
				//DrawFormatStringF(250,250,RED,"TIMER %f",newtimer2->GetTimerSec());
				//DrawFormatStringF(250,270,RED,"TIMER %f",newtimer2->GetTimerMS());
				newtimer2->Update();
				SetFontSize(FONTSIZE_TITLE);
				DrawStringF(WINDOW_W / 2 - 350, WINDOW_H - 150, "CLICK TO ENTER", RED);
				SetFontSize(FONTSIZE_NORMAL);
				if (IsAdvanceCutsceneButton() && newtimer2->IsTimerSec(1.0f))
				{
					isCutsceneOver = true;
				}
			}
				//static std::unique_ptr<TalkBox> tbox3 = std::make_unique<TalkBox>(800.0f, "and my daughter is in danger", "John");
				//tbox3->RenderDialogBox();
		}

		}
		break;

	case CutsceneGameEnding :
		if (!fadebox_ptr->IsFadeIn())
		{
		static std::unique_ptr<TalkBox> tbox1_ = std::make_unique<TalkBox>(600.0f, "My name is John", "John");
		tbox1_->RenderDialogBox();
		if (tbox1_->isDone)
		{
			static std::shared_ptr<Timer> newtimer = std::make_shared<Timer>();
			static std::unique_ptr<TalkBox> tbox2_ = std::make_unique<TalkBox>(600.0f, "And today I saved my daughter", "John");
			tbox2_->RenderDialogBox();
			if (tbox2_->isDone) { newtimer->Update(); }
			if (IsAdvanceCutsceneButton() && newtimer->IsTimerSec(2.0f))
			{
				DrawStringF(WINDOW_W_HALF - 180, WINDOW_H_HALF + 260, creditslist, RED, 1);
				isCutsceneOver = true;
			}
		}
		}
		break;
	}
}
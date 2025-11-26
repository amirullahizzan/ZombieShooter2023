#include "WinMain.h"

#include "../customsrc/Loader.h"
#include "../customsrc/DebugTools.h"
#include "../customsrc/Pos.h"
#include "../customsrc/Primitive.h"
#include "../customsrc/Hitbox.h"
#include "../customsrc/SceneManager.h"
#include "../customsrc/PauseMenu.h"
#include "../customsrc/GameTimer.h"
#include "../customsrc/Entity.h"
#include "../customsrc/CombatPacket.h"
#include "../customsrc/Gauge.h"
#include "../customsrc/UpgradePacket.h"

namespace calculate
{
	double DampenAngle(double targetAngle)
	{
		constexpr double dampingFactor = 0.02; // Adjust this value (should be < 1)

		static double currentAngle = targetAngle; // Initialize with the target angle

		// Calculate the angle change based on delta time
		double angleChange = (targetAngle - currentAngle);

		if (angleChange > PI)
		{
			angleChange -= 2 * PI;
		}
		else if (angleChange < -PI)
		{
			angleChange += 2 * PI;
		}

		// Apply damping directly to the angle change using deltaTime
		double FPS_factor = 1.0 - pow(dampingFactor, time.deltaTime);
		double adjustedDampen = FPS_factor * angleChange;

		// Update the current angle with the damped change
		currentAngle += adjustedDampen;

		return currentAngle;
	}
	double AngleToRadian(double angle)
	{
		return angle * (PI / 180.0);
	}
	
	double RadianToAngle(double radian)
	{
		return radian * (180.0f / PI);
	}

	double RotateToTarget(Float2 objectLookingatMouse, const Float2& _targetPos)
	{
		double px = _targetPos.x - objectLookingatMouse.x;
		double py = _targetPos.y - objectLookingatMouse.y;

		double angleRad = atan2(py, px) + PI * 0.5;

		return calculate::DampenAngle(angleRad);
	}

}

Loader loader;
Time time;

//GetDrawStringWidth

//######################
//Main Definition

Float2 campos;
extern bool isWindowedMode;
static float distance_traveled = 0;

//ESCAPE FROM THE DARK LABYRINTH
#include "../customsrc/CustomGameMain.h"

//GLOBAL Decl HERE
//static int currentRoom = 0;

DebugTools debugtools;
UIManager ui;
Player player;
bool isCutsceneOver = true;
int tutorialSwitch = 0;

DialogueSystem talkdialogue1;
DialogueSystem* currentDialogue = &talkdialogue1;
PauseMenu pausemenu = { {WINDOW_W_HALF,50},{WINDOW_W_HALF,WINDOW_H - 50} , {0,0} , {WINDOW_W,WINDOW_H} };

	enum SceneModeEnum
	{
		InitDisclaimerScene = 0,
		DisclaimerScene = 1,
		InitMainMenuScene = 2,
		MainMenuScene = 3,
		InitGameplayScene = 4,
		GameplayScene = 5,

		InitGameWinScene = 8, 
		GameWinScene = 9, 
		GameOverScene,

		InitCutsceneIntroScene,
		Cutscene,
	};


//int currentScene = InitDisclaimerScene;
int currentScene = InitGameplayScene;
//int currentScene = InitCutsceneIntro;

enum CutsceneModeEnum
{
	CutsceneIntro = 1,
	CutsceneGameEnding = 2
};

int cutsceneMode = 0;

void GameInit()
{
	//AddFontFile(data/.fnt);
	loader.Load();
}

Float2 mousePos;
extern std::shared_ptr<Enemy> currentEnemy[];

std::shared_ptr<FadeBox> fadebox_ptr = std::make_shared<FadeBox>(FadeBox::FadeModeEnum::NoMode);
void GameUpdate()
{
	time.UpdateTick();
	mousePos.x = (static_cast<float>(GetMouseX()));
	mousePos.y = (static_cast<float>(GetMouseY()));
	switch (GetSceneMode())
	{
	case InitDisclaimerScene:
		pausemenu.isPauseAllowed = false;
		loader.InitFadeInMusic_Volume();
		fadebox_ptr->SetFadeIn(3);
		currentScene = DisclaimerScene;
		break;
	case DisclaimerScene:
		break;
	case InitMainMenuScene:
		if(!CheckSoundMem(loader.terror_bgm_handler)	) 	{		PlaySoundMem(loader.terror_bgm_handler, DX_PLAYTYPE_LOOP);	}
		InitMainMenu();
		fadebox_ptr->SetFadeIn(4);
		currentScene = MainMenuScene;
		tutorialSwitch = 0;
		break;

	case MainMenuScene:
		loader.FadeInMusic_Volume();
		UpdateMainMenu(mousePos);
		break;

	case InitGameplayScene:
	
		loader.SetSoundsVolume();
		InitGameplay();
		StopSoundMem(loader.terror_bgm_handler);
		StopSoundMem(loader.scary1_bgm_handler);
		if (GetSoundCurrentTime(loader.room_bgm_handler) == 0) 
		{ 
			PlaySoundMem(loader.scary1_bgm_handler, DX_PLAYTYPE_LOOP, 1);
		}
		fadebox_ptr->SetFadeIn(2);
		pausemenu.isPauseAllowed = true;
		currentScene = GameplayScene;
		break;

	case GameplayScene:
		if (!isGamePaused && !pausemenu.isGameTutorial)
		{
			player.Update();
			UpdateGameplay();
		}
		UpdateTutorial();
		if (GetKeyPush(KEY_INPUT_ESCAPE) && !pausemenu.isGameTutorial) { isGamePaused = !isGamePaused; }
		break;
	
	case InitCutsceneIntroScene:
		fadebox_ptr->SetFadeIn(3);
		cutsceneMode = CutsceneModeEnum::CutsceneIntro;
		InitCutscene();
		SetSceneMode(Cutscene);
		break;

	case InitGameWinScene:
		fadebox_ptr->SetFadeIn(3);
		cutsceneMode = CutsceneModeEnum::CutsceneGameEnding;
		InitCutscene();
		SetSceneMode(Cutscene);
		break;

	case Cutscene:
		UpdateCutscene();
		break;

	case GameOverScene:
		UpdateGameOver();
		break;
	}
	if (!isCutsceneOver)
	{ UpdateCutscene(); }
	fadebox_ptr->Update();
}

void GameDraw()
{
	switch (GetSceneMode())
	{
	case InitDisclaimerScene:
		break;
	case DisclaimerScene:
		RenderDisclaimer();

		break;
	case InitMainMenuScene:
		currentScene = MainMenuScene;
		break;

	case MainMenuScene:
		RenderMainMenu();
		break;

	case InitGameplayScene:

		break;
	case GameplayScene:
		RenderGameplay();
		pausemenu.RenderPause();
		//player can choose where to turn in the labyrinth
		break;

		//CUTSCENES#
	case InitCutsceneIntroScene :
		break;

	case Cutscene:
		RenderCutscene();
		break;

	case GameOverScene:
		RenderGameOver();
		break;
	}

	fadebox_ptr->RenderFade();
	GameDebug();
}

void GameExit()
{


}

//############################################################################
//Disclaimer
//const int DISCLAIMERLINES = 2;
//TCHAR disclaimer[DISCLAIMERLINES][1000] =
TCHAR disclaimer[] =
{
("      This game may contain disturbing contents and unsettling themes.\n \n \n"
"If you are sensitive, we strongly recommend that you refrain from playing.\n \n \n"
"                        Viewer discretion is advised.\n \n \n"
)
,
//("           This game may contain disturbing contents and unsettling themes.\n \n"
//"If you are sensitive, we strongly recommend that you refrain from playing.\n \n"
//"                        Viewer discretion is advised.\n \n"
//)
};


void RenderDisclaimer()
{
	SetFontSize(FONTSIZE_NORMAL);

	DrawStringF(WINDOW_W_HALF - 100, WINDOW_H_HALF - 280, "[DISCLAIMER]", WHITE, 1);
	DrawStringF(WINDOW_W_HALF - 560, WINDOW_H_HALF - 160, disclaimer, GRAY, 1);
	//if (time.BoolInvoker(10))
	//static Timer* disclaimerTimer = new Timer();
	// should this be static?
	static std::unique_ptr<Timer> disclaimerTimer = std::make_unique<Timer>();
	//disclaimerTimer->Update();
	if(disclaimerTimer->IsTimerSec(3))
	{
		static bool isClicked = false;
		if (GetMouseInput(MOUSE_INPUT_LEFT))
		{
			fadebox_ptr->SetFadeOut(3);
			isClicked = true;
		}
		if (!fadebox_ptr->IsFadeOut() && isClicked)
		{
			SetSceneMode(InitMainMenuScene);
		}
		static int blendparam = 0;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendparam);
		if (blendparam <= 255) { blendparam++; }
		DrawStringF(WINDOW_W_HALF - 160, WINDOW_H_HALF + 140, "BY CHOOSING TO CONTINUE, ", WHITE);
		DrawStringF(WINDOW_W_HALF - 580, WINDOW_H_HALF + 200, "I HEREBY ACKNOWLEDGE AND AGREE TO EXPERIENCE POTENTIALLY DISTURBING CONTENT.", RED, 1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (disclaimerTimer->IsTimerSec(5))
	{
		static int blendparam = 0;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, blendparam);
		if (blendparam <= 255) { blendparam++; }
		DrawStringF(WINDOW_W_HALF - 120, WINDOW_H_HALF + 260, "CLICK TO CONTINUE.", RED, 1);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	
}

//##############################
//MainMenu

enum SelectionsIndex
{
	StartGame = 0,
	HowToPlay = 1,
	Settings = 2,
	ExitGame = 3,
};

const float BUTTON_WIDTH = 500;
const float BUTTON_HEIGHT = 50;
Button startgameButton = { {WINDOW_W / 2,WINDOW_H / 2 + 110}, BUTTON_WIDTH , BUTTON_HEIGHT };
//Button howtoplayButton = { {WINDOW_W/2,WINDOW_H/2 + 200}, BUTTON_WIDTH, BUTTON_HEIGHT };
Button settingsButton = { {WINDOW_W / 2,WINDOW_H / 2 + 190}, BUTTON_WIDTH, BUTTON_HEIGHT };
Button exitgameButton = { {WINDOW_W / 2,WINDOW_H / 2 + 270}, BUTTON_WIDTH, BUTTON_HEIGHT };

const static int MAX_MENU_BUTTONS = 3;
static Button menuButtons[MAX_MENU_BUTTONS] =
{
	startgameButton ,
	settingsButton,
	exitgameButton
};
const TCHAR* menuText[MAX_MENU_BUTTONS] =
{
	"START GAME","SETTING","EXIT GAME"
};

Button settingsPauseButton{ {WINDOW_W / 2,WINDOW_H / 2 + 100}, BUTTON_WIDTH , BUTTON_HEIGHT };
Button exitgamePauseButton{ {WINDOW_W / 2,WINDOW_H / 2 + 100}, BUTTON_WIDTH , BUTTON_HEIGHT };
const static int MAX_PAUSEMENU_BUTTONS = 2;
static Button pauseButtons[MAX_PAUSEMENU_BUTTONS] =
{
	settingsPauseButton,
	exitgamePauseButton
};
const TCHAR* pausemenuText[MAX_PAUSEMENU_BUTTONS] =
{
	"SETTING","EXIT GAME"
};

struct MainMenu
{
	bool isclickedStartGame = false;
	bool introEffectDone = false;
	float dramaticeffecttimer = 2.5f;
	float dramaticzoom = 0;

	void IntroEffect()
	{
		DrawExtendGraphF(0- dramaticzoom, 0- dramaticzoom, WINDOW_W+ dramaticzoom, WINDOW_H+ dramaticzoom, loader.background_handler, 0);
		dramaticzoom += time.deltaTime*50;

		dramaticeffecttimer-= time.deltaTime;
		if (dramaticeffecttimer <= 0.0)
		{
			introEffectDone = true;
		}
	}

};
MainMenu mainmenu;
std::unique_ptr<MainMenu> mainmenu_p = nullptr;

void InitMainMenu()
{
	mainmenu_p = std::make_unique<MainMenu>();
	//PlayMusic();
	ui.SetSelectorCursorPosY(menuButtons[0].pos1.y);
}

void UpdateMainMenu(const Float2& mousepos)
{
	//######################
	// BUTTONS
	//######################

	enum MenuButtonsIndexEnum
	{
		startgameButton,
		settingsButton,
		exitgameButton
	};

	for (int i = 0; i < MAX_MENU_BUTTONS; i++)
	{
		if (HitboxPointBox(mousepos, menuButtons[i], 0) && !pausemenu.IsInMenu())
		{
			ui.SetSelectorCursorPosY(menuButtons[i].pos1.y);
			menuButtons[i].SetIsAnimTrue();
			if (GetMouseInput(MOUSE_INPUT_LEFT))
			{
				switch (i)
				{
				case startgameButton:
					//PlaySound(); // Play custom game start sound
					fadebox_ptr->SetFadeOut(3);
					mainmenu.isclickedStartGame = true;
					break;
				case settingsButton:
					pausemenu.OpenSetting();
					break;
				case exitgameButton:
					exit(0);
					break;
				}
			}
		}
		else
		{
			menuButtons[i].SetIsAnimFalse();
		}
	}
	if (mainmenu.isclickedStartGame)
	{
		loader.FadeOutMusic_Volume();
	}
	if (mainmenu.introEffectDone)
	{
			if (!fadebox_ptr->IsFadeOut()) 
			{ SetSceneMode(InitCutsceneIntroScene); }
	}
	pausemenu.UpdateSetting(mousepos,MOUSE_RADIUS);
}

void RenderMainMenu()
{
	if (!mainmenu.isclickedStartGame)
	{
	//DrawExtendGraph(0, 50, WINDOW_W, 450, loader.background_handler, 0);
	DrawExtendGraph(0, 0, WINDOW_W, WINDOW_H, loader.background_handler, 0);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA,200);
	DrawBoxAA(0,450,WINDOW_W,WINDOW_H-20,BLACK,1);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	//int cursorIndex = 0;
	// or use class
	SetFontSize(FONTSIZE_NORMAL);
	for (int i = 0; i < MAX_MENU_BUTTONS; i++)
	{
		//DEBUG //DrawBox(menuButtons[i].pos1.x, menuButtons[i].pos1.y, menuButtons[i].pos2.x, menuButtons[i].pos2.y,GREEN,1);
		menuButtons[i].RenderSelection(loader.icon_selector_handler);
		float textoffset_y = menuButtons[i].height / 2 - 10;
		DrawStringF(menuButtons[i].width, menuButtons[i].pos1.y + textoffset_y, menuText[i], menuButtons[i].GetTextColor(), 0);
	}
	ui.RenderCursor(WINDOW_W_HALF - 250, ui.GetSelectorCursorPosY());
	SetFontSize(FONTSIZE_TITLE);
	Float2 titlepos = { WINDOW_W_HALF - 225,WINDOW_H_HALF - 180 };
	DrawStringF(titlepos.x - 2, titlepos.y - 2, "LABYRINTHOS", BLACK, 1);
	DrawStringF(titlepos.x + 2, titlepos.y + 2, "LABYRINTHOS", BLACK, 1);
	DrawStringF(titlepos.x, titlepos.y, "LABYRINTHOS", RED, 1);
	SetFontSize(FONTSIZE_SMALL);
	pausemenu.RenderSetting();
	}
	if (mainmenu.isclickedStartGame)
	{
		mainmenu.IntroEffect();
	}
}

//######################
// CUTSCENE

#include "../scenes/Cutscene.h"



//######################
//Gameplay

#include "../scenes/Gameplay.h"

//######################
//GameOver
void UpdateGameOver()
{

}

void RenderGameOver()
{
	DrawStringF(WINDOW_W_HALF, WINDOW_H_HALF,"Game Over", RED);
	//laugh
}


//######################
void GameDebug()
{
	//debugtools.DrawXGrid();
	//debugtools.DrawYGrid();

	std::string timestring = std::to_string(time.deltaTime);

	printfDx("Scene : %d\n", GetSceneMode());
	printfDx("time deltatime: %f\n", time.deltaTime);
	//printfDx("delaytimer : %f\n", gun.delaytimer);
	//printfDx("ammo : %d\n", *gun.currentammo);
	printfDx("spawn timer : %f\n", 	spawner.spawntimer);
	printfDx("spawn timer : %f\n", 	spawntimer_modifier);
	printfDx("battle stagenum  : %f\n", 	stagemanager.GetStageNum());
	printfDx("Meters travelled : %f\n", distance_traveled);

	//printfDx("player exp : %f\n", zombieSpawner.spawntimer);
	


	//DrawBox(startgameButton.pos1.x, startgameButton.pos1.y, startgameButton.pos2.x, startgameButton.pos2.y, RED, 1);
}

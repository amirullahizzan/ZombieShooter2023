#include "../src/WinMain.h"
#include "Hitbox.h"
#include "SceneManager.h"

//############################
//## Decl 
//############################


int GetSceneMode()
{
	return currentScene;
}

void SetSceneMode(int sceneIndex)
{
	currentScene = sceneIndex;
}

int dialogueIndex = 0;
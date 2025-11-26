#pragma once

void GameDebug();
void RenderDisclaimer();
void InitMainMenu();
void UpdateMainMenu(const Float2& mousepos);
void RenderMainMenu();
void InitGameplay();
void UpdateGameplay();
void RenderGameplay();

void UpdateGameOver();
void RenderGameOver();

void UpdateTutorial();
//
void InitCutscene();
void UpdateCutscene();
void RenderCutscene();

void MovementTutorial();
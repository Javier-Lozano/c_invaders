#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <stdbool.h>
#include "defines.h"
#include "system.h"

////// Data Structures

typedef enum {
	SCENE_TITLE,
	SCENE_PLAY,
	SCENE_CREDITS
} GameScene;

//// Globals

extern GameScene	g_GameScene;
extern SDL_Event	g_Event;
extern bool		g_MainLoop;

///// Functions

void Game_SceneTitle();
void Game_ScenePlay();
void Game_SceneCredits();

// Background Starfield
void Game_Background();

#endif //_GAME_SCENE_H_

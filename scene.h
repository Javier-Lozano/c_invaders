#ifndef _GAME_SCENE_H_
#define _GAME_SCENE_H_

#include <stdbool.h>
#include "defines.h"
#include "rendering.h"
#include "resources.h"

////// Data Structures

typedef enum {
	SCENE_TITLE,
	SCENE_PLAY,
	SCENE_CREDITS,
	SCENE_EXIT
} GameScene;

//// Globals

extern GameScene	g_GameScene;
extern Resources	g_Resources;
extern SDL_Renderer	*g_Renderer;
extern SDL_Color g_Colors[C_COUNT];

///// Functions

void Scene_Title();
void Scene_Play();
void Scene_Credits();

void Scene_Background();

#endif //_GAME_SCENE_H_

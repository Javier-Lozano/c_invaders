#ifndef _GAME_SYSTEM_H_
#define _GAME_SYSTEM_H_

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "defines.h"

///// Globals

extern SDL_Window	*g_Window;
extern SDL_Renderer	*g_Renderer;
extern SDL_Texture	*g_Target;
extern SDL_Rect		g_TargetRectSrc;
extern SDL_Rect		g_TargetRectDst;
extern int			g_Scale;

///// Data Structures

typedef struct {
	SDL_Texture *textures;
	// Sound
	// Sprites *sprites;
} ResourceManager;

///// Functions

// SDL
bool Game_SDL_Start();
void Game_SDL_End();

// Resource
bool Game_LoadResources(ResourceManager *manager);

// Rendering
void Game_RenderPrepare();
void Game_Render();
void Game_ScaleTarget(int s);
void Game_ToggleFullscreen();

#endif //_GAME_SYSTEM_H_

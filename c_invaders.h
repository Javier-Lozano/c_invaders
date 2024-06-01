#ifndef _C_INVADERS_H_
#define _C_INVADERS_H_

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "input.h"

/***** Macros *****/

#define WIN_W 224
#define WIN_H 256
#define SCALE 2

/***** Globals *****/

// SDL
extern SDL_Window   *g_Window;
extern SDL_Renderer *g_Renderer;
// Rendering Target
//extern Target       g_Target;
// Scene
//extern Uint8        g_Scene;
// Input
extern Input        g_Input;
// Control
extern bool         g_MainLoop;
extern double       g_DeltaTime;

///// Types

//typedef struct {
//	SDL_Texture *texture;
//	SDL_Rect    rect;
//	Uint8       scale;
//} Target;

/***** Functions *****/

// init.c
bool Game_Init();
void Game_Close();

// events.c
void Game_HandleEvents();

#endif // _C_INVADERS_H_


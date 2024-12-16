#ifndef RENDERING_H_
#define RENDERING_H_

#include "SDL.h"

/***** MACROS *****/

#define WINDOW_TITLE "C Invaders"
#define WINDOW_W (240)
#define WINDOW_H (320)

/***** Functions *****/

int InitSDL();
void CloseSDL();

SDL_Window   *GetWindow();
SDL_Renderer *GetRenderer();

#endif // RENDERING_H_

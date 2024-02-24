#ifndef _RENDERING_H_
#define _RENDERING_H_

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "defines.h"

///// Functions
bool Rendering_Init();
void Rendering_End();

void Rendering_Prepare();
void Rendering_Present();
void Rendering_SetScale(int scale);
void Rendering_ToggleFullscreen();

#endif // _RENDERING_H_

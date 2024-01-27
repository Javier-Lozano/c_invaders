#ifndef _INPUT_H_
#define _INPUT_H_

#include <SDL2/SDL.h>
#include "defines.h"

enum InputShifts {
	INPUT_LEFT,
	INPUT_RIGHT,
	INPUT_FIRE,
	INPUT_PAUSE,
	INPUT_SIZE
};

extern Uint8 g_Input;
extern SDL_Scancode g_Bindings[INPUT_SIZE];

void HandleInput(SDL_Event *event);

#endif // _INPUT_H_

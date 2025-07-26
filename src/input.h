#ifndef INPUT_H_
#define INPUT_H_

#include "SDL.h"
#include <stdbool.h>

typedef enum {
	STATE_UP       = 0,
	STATE_DOWN     = 1,
	STATE_PRESSED  = 2,
	STATE_RELEASED = 4
} ButtonState;

void UpdateInputState();
void ClearInputState();

bool CheckState(SDL_Scancode scancode, ButtonState state);

bool IsAnyKeyPressed();
bool IsKeyDown(SDL_Keycode key);
bool IsKeyPressed(SDL_Keycode key);
bool IsKeyReleased(SDL_Keycode key);

#endif // INPUT_H_

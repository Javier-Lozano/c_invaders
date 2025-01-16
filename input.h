#ifndef INPUT_H_
#define INPUT_H_

#include "SDL.h"
#include "common_types.h"

typedef enum {
	STATE_UP       = 0,
	STATE_DOWN     = 1,
	STATE_PRESSED  = 2,
	STATE_RELEASED = 4
} PressState;

typedef enum {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_FIRE,
	BUTTON_CANCEL,
	BUTTON_COUNT,
} Button;

void ProcessInput();
void ClearInput();

void BindKeyToButton(SDL_Keycode key, Button button);

bool CheckState(SDL_Scancode scancode, PressState state);
bool IsAnyPressed();
bool IsKeyDown(SDL_Keycode key);
bool IsKeyPressed(SDL_Keycode key);
bool IsKeyReleased(SDL_Keycode key);

#endif // INPUT_H_

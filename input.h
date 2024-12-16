#ifndef INPUT_H_
#define INPUT_H_

#include "SDL.h"

/***** Enums *****/

typedef enum {
	STATE_UP       = 0,
	STATE_DOWN     = 1,
	STATE_PRESSED  = 2,
	STATE_RELEASED = 4
} InputState;

typedef enum {
	BUTTON_LEFT,
	BUTTON_MIDDLE,
	BUTTON_RIGHT,
	BUTTON_COUNT
} MouseButton;

/***** Function *****/

void ProcessInput();

int IsKeyDown(SDL_Keycode key);
int IsKeyPressed(SDL_Keycode key);
int IsKeyReleased(SDL_Keycode key);

int IsMSBDown(MouseButton button);
int IsMSBPressed(MouseButton button);
int IsMSBReleased(MouseButton button);

void GetMousePosition(int *x, int *y);
void GetMouseDelta(int *x, int *y);

#endif // INPUT_H_

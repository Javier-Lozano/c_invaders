#include "input.h"

struct input_st {
	u8 key_state[SDL_NUM_SCANCODES];
	u8 msb_state[BUTTON_COUNT];
	SDL_Point mouse_position;
	SDL_Point mouse_delta;
} static g_Input;

static int CheckScancode(SDL_Scancode scancode, PressState state);
static int CheckMouseButton(MouseButton button, PressState state);

void ProcessInput()
{
	// Store Mouse and Keyboard previous "Press Down" State and
	// compare it with the current State. This allows for checking
	// if a key or button was just pressed or released

	int mouse_x, mouse_y;
	const u8  sdl_msb_state = SDL_GetMouseState(&mouse_x, &mouse_y);
	const u8 *sdl_key_state = SDL_GetKeyboardState(NULL);

	// Keyboard
	for(int i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		u8 state = 0;
		if (sdl_key_state[i])
		{
			state = STATE_DOWN;
			if (!CheckScancode(i, STATE_DOWN))
				state |= STATE_PRESSED;
		}
		else if (CheckScancode(i, STATE_DOWN))
		{
			state = STATE_RELEASED;
		}

		g_Input.key_state[i] = state;
	}

	// Mouse Buttons
	for(int i = 0; i < BUTTON_COUNT; i++)
	{
		u8 state = 0;
		if (sdl_msb_state & (1 << i))
		{
			state = STATE_DOWN;
			if (!CheckMouseButton(i, STATE_DOWN))
				state |= STATE_PRESSED;
		}
		else if (!CheckMouseButton(i, STATE_DOWN))
		{
				state = STATE_RELEASED;
		}

		g_Input.msb_state[i] = state;
	}

	// Mouse Position and Delta
	g_Input.mouse_delta.x = g_Input.mouse_position.x - mouse_x;
	g_Input.mouse_delta.y = g_Input.mouse_position.y - mouse_y;
	g_Input.mouse_position.x = mouse_x;
	g_Input.mouse_position.y = mouse_y;
}

// Keyboard

static int CheckScancode(SDL_Scancode scancode, PressState state)
{
	return (g_Input.key_state[scancode] & state) == state;
}

int IsKeyDown(SDL_Keycode key)
{
	return CheckScancode(SDL_GetScancodeFromKey(key), STATE_DOWN);
}

int IsKeyPressed(SDL_Keycode key)
{
	return CheckScancode(SDL_GetScancodeFromKey(key), STATE_PRESSED);
}

int IsKeyReleased(SDL_Keycode key)
{
	return CheckScancode(SDL_GetScancodeFromKey(key), STATE_RELEASED);
}

// Mouse

static int CheckMouseButton(MouseButton button, PressState state)
{
	return (g_Input.msb_state[button] & state) == state;
}

int IsMSBDown(MouseButton button)
{
	return CheckMouseButton(button, STATE_DOWN);
}

int IsMSBPressed(MouseButton button)
{
	return CheckMouseButton(button, STATE_PRESSED);
}

int IsMSBReleased(MouseButton button)
{
	return CheckMouseButton(button, STATE_RELEASED);
}

void GetMousePosition(int *x, int *y)
{
	if (x != NULL)
		*x = g_Input.mouse_position.x;

	if (y != NULL)
		*y = g_Input.mouse_position.y;
}

void GetMouseDelta(int *x, int *y)
{
	if (x != NULL)
		*x = g_Input.mouse_delta.x;

	if (y != NULL)
		*y = g_Input.mouse_delta.y;
}


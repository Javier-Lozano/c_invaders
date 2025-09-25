#include "input.h"

static char g_KeyState[SDL_NUM_SCANCODES];
static bool g_AnyKeyPressed;

void InputStateUpdate()
{
	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

	g_AnyKeyPressed = false;

	for(int i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		char state = 0;
		if (keyboard[i] > 0)
		{
			state = STATE_DOWN;
			if ((g_KeyState[i] & STATE_DOWN) == 0)
			{
				state |= STATE_PRESSED;
				g_AnyKeyPressed = true;
			}
		}
		else if ((g_KeyState[i] & STATE_DOWN) == STATE_DOWN)
		{
			state = STATE_RELEASED;
		}

		g_KeyState[i] = state;
	}
}

void InputStateClear()
{
	// Remove key press and release states
	g_AnyKeyPressed = false;
	for(int i = 0; i < SDL_NUM_SCANCODES; i++)
		g_KeyState[i] &= STATE_DOWN;
}

bool CheckState(SDL_Scancode scancode, ButtonState state)
{
	return (g_KeyState[scancode] & state) == state;
}

bool IsAnyKeyPressed()
{
	return g_AnyKeyPressed;
}

bool IsKeyDown(SDL_Keycode key)
{
	return CheckState(SDL_GetScancodeFromKey(key), STATE_DOWN);
}

bool IsKeyPressed(SDL_Keycode key)
{
	return CheckState(SDL_GetScancodeFromKey(key), STATE_PRESSED);
}

bool IsKeyReleased(SDL_Keycode key)
{
	return CheckState(SDL_GetScancodeFromKey(key), STATE_RELEASED);
}

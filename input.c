#include "input.h"

static i8   g_KeyState[SDL_NUM_SCANCODES];
static u32  g_KeyBindings[BUTTON_COUNT];
static bool g_AnyPressed;

void ProcessInput()
{
	const u8 *sdl_key_state = SDL_GetKeyboardState(NULL);

	bool any_key = false;

	for(int i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		i8 state = 0;
		if (sdl_key_state[i] > 0)
		{
			state = STATE_DOWN;
			if ((g_KeyState[i] & STATE_DOWN) == 0)
			{
				state |= STATE_PRESSED;
				any_key = true;
			}
		}
		else if ((g_KeyState[i] & STATE_DOWN) == STATE_DOWN)
		{
			state = STATE_RELEASED;
		}

		g_KeyState[i] = state;
	}

	g_AnyPressed = any_key;
}

void ClearInput()
{
	g_AnyPressed = false;
	for(int i = 0; i < SDL_NUM_SCANCODES; i++)
	{
		g_KeyState[i] &= STATE_DOWN;
	}
}

void BindKeyToButton(SDL_Keycode key, Button button)
{
	SDL_Scancode sc = SDL_GetScancodeFromKey(key);
	g_KeyBindings[button] = sc;
}

bool CheckState(SDL_Scancode scancode, PressState state)
{
	return (g_KeyState[scancode] & state) == state;
}

bool IsAnyPressed()
{
	return g_AnyPressed;
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


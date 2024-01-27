#include "input.h"

void HandleInput(SDL_Event *event)
{
	if (event->type == SDL_KEYDOWN || event->type == SDL_KEYUP)
	{
		for (int i = 0; i < INPUT_SIZE; i++)
		{
			if (event->key.keysym.scancode == g_Bindings[i])
			{
				g_Input = (event->type == SDL_KEYDOWN) ? (g_Input | (1 << i)) : (g_Input & ~(1 << i));
			}
		}
	}
}

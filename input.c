#include "c_invaders.h"

/***** Globals *****/

Input g_Input;

/***** Functions *****/

void Input_UpdateInput()
{
	const Uint8 *keyboard = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < INPUT_COUNT; i++)
	{
		Uint8 state = 0;

		if (keyboard[g_Input.key_map[i]])
		{
			state |= INPUT_HELD;

			if ( !(CHECK_HELD(i)) )
			{
				state |= INPUT_PRESSED;
			}
		}
		else
		{
			if (CHECK_HELD(i))
			{
				state |= INPUT_RELEASED;
			}
		}

		g_Input.key_state[i] = state;
	}
}

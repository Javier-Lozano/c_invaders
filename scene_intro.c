#include "c_invaders.h"

static void HandleInput(Uint8 *input, SDL_Scancode *keybindings);

void Scene_Intro(GameState *game_state)
{
	// Variables
	Uint8 input;
	Uint8 selection = 0;

	float timer = 0;

	while(game_state->is_running)
	{
		// Handle Events
		Game_HandleEvents(game_state);
		// Get Input
		HandleInput(&input, game_state->keybindings);

		timer += 1.0f * game_state->delta_time;

		printf("(float) Timer: %f\n", timer);


	}
}

static void HandleInput(Uint8 *input, SDL_Scancode *keybindings)
{
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	*input = 0;

	for(int i = 0; i < INPUT_COUNT; i++)
	{
		if (keystate[keybindings[i]] > 0)
		{
			*input = INPUT_SET(*input, i);
		}
	}
}


#include "c_invaders.h"

void Game_HandleEvents(GameState *game_state)
{
	// Variables
	static Uint64 previous;
	static Uint64 current;
	SDL_Event e;

	// Get Delta Time
	if (previous == 0) { current = SDL_GetPerformanceCounter(); }
	previous = current;
	current = SDL_GetPerformanceCounter();
	game_state->delta_time = (double) (current - previous) / (double) SDL_GetPerformanceFrequency();

	// Process Events
	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_QUIT:
				game_state->is_running = false;
				break;
			case SDL_WINDOWEVENT:
				break;
		}
	}
}

#include "init.h"

bool Game_Init()
{
	// Init. SDL
	if (SDL_Init(SDL_INIT_FLAGS) < 0)
	{
		fprintf(stderr, "Error: Couldn't init. SDL. %s\n", SDL_GetError());
		return false;
	}

	// Init. Rendering
	if (!Rendering_Init() | !Resources_Init())
	{
		fprintf(stderr, "Failed to initiate game.\n");
		return false;
	}

	// Init. Resources

	return true;
}

void Game_End()
{
	Rendering_End();
	SDL_Quit();
}

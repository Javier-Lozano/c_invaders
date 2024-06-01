#include "c_invaders.h"

/***** Globals *****/

double g_DeltaTime;

/***** Functions *****/

static void ProcessDeltaTime();

void Game_HandleEvents()
{
	// Varibales
	SDL_Event e;

	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_QUIT:
				g_MainLoop = false;
				break;
		}
	}

	// Update Player Input
	Input_UpdateInput();

	// Delta Time
	ProcessDeltaTime();
}

static void ProcessDeltaTime()
{
	static Uint64 past;
	static Uint64 present;

	if (past == 0) { present = SDL_GetPerformanceCounter(); }
	past = present;
	present = SDL_GetPerformanceCounter();
	g_DeltaTime = (double)(present - past) / (double)SDL_GetPerformanceFrequency();
}

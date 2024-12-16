#include "events.h"

/***** Globals *****/

static double g_TimeDelta;

/***** Functions (Prototypes) *****/

static void ProcessTimeDelta();

/***** Functions *****/

void ProcessEvents(GameContext *ctx)
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			ctx->is_running = 0;
	}

	ProcessInput();
	ProcessTimeDelta();
}

static void ProcessTimeDelta()
{
	static Uint64 time_past;
	static Uint64 time_present;

	if (time_past == 0)
		time_past = SDL_GetPerformanceCounter();

	time_present = SDL_GetPerformanceCounter();
	g_TimeDelta = (double)(time_present - time_past) / (double)SDL_GetPerformanceFrequency();
	time_past = time_present;
}

double GetTimeDelta()
{
	return g_TimeDelta;
}

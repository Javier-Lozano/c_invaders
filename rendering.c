#include "rendering.h"

/***** Globals *****/

static SDL_Window   *g_Window;
static SDL_Renderer *g_Renderer;

/***** Functions *****/

int InitSDL()
{
	SDL_Window   *window;
	SDL_Renderer *renderer;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
		goto sdl2_init_error;

	window = SDL_CreateWindow(WINDOW_TITLE, 0, 0, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN);
	if (window == NULL)
		goto sdl2_init_error;

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
		goto sdl2_init_error;

	g_Window   = window;
	g_Renderer = renderer;

	return 1;

sdl2_init_error:
	printf("Couldn't initialize SDL2. %s\n", SDL_GetError());
	return 0;
}

void CloseSDL()
{
	SDL_DestroyRenderer(g_Renderer);
	SDL_DestroyWindow(g_Window);
	SDL_Quit();
}

SDL_Window *GetWindow()
{
	return g_Window;
}

SDL_Renderer *GetRenderer()
{
	return g_Renderer;
}

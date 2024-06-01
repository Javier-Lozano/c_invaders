#include "c_invaders.h"

/***** Globals *****/

SDL_Window   *g_Window;
SDL_Renderer *g_Renderer;
bool         g_MainLoop;

/***** Functions *****/

bool Game_Init()
{
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Error: Couldn't init SDL. %s\n", SDL_GetError());
		return false;
	}

	g_Window = SDL_CreateWindow("C Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
	if (g_Window == NULL)
	{
		fprintf(stderr, "Error: Couldn't create Window. %s\n", SDL_GetError());
		return false;
	}
	
	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDERER_ACCELERATED);
	if (g_Renderer == NULL)
	{
		fprintf(stderr, "Error: Couldn't create Renderer. %s\n", SDL_GetError());
		return false;
	}

	// Input
	g_Input.key_map[INPUT_UP] = SDL_SCANCODE_UP;
	g_Input.key_map[INPUT_DOWN] = SDL_SCANCODE_DOWN;
	g_Input.key_map[INPUT_LEFT] = SDL_SCANCODE_LEFT;
	g_Input.key_map[INPUT_RIGHT] = SDL_SCANCODE_RIGHT;
	g_Input.key_map[INPUT_FIRE] = SDL_SCANCODE_X;
	g_Input.key_map[INPUT_CANCEL] = SDL_SCANCODE_Z;

	// Control
	g_MainLoop = true;

	return true;
}

void Game_Close()
{
	SDL_DestroyRenderer(g_Renderer);
	SDL_DestroyWindow(g_Window);

	SDL_Quit();
}

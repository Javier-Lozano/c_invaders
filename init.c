#include "c_invaders.h"

///// Functions

static int InitSDL(GameState *game_state);
static int InitTarget(Target *target, SDL_Renderer *renderer);

int Game_Init(GameState *game_state)
{
	// Init in bundle
	if (!InitSDL(game_state) || !InitTarget(&game_state->target, game_state->renderer))
	{
		printf("NOPE\n");
		return 0;
	}

	game_state->keybindings[INPUT_UP]     = SDL_SCANCODE_UP;
	game_state->keybindings[INPUT_DOWN]   = SDL_SCANCODE_DOWN;
	game_state->keybindings[INPUT_LEFT]   = SDL_SCANCODE_LEFT;
	game_state->keybindings[INPUT_RIGHT]  = SDL_SCANCODE_RIGHT;
	game_state->keybindings[INPUT_FIRE]   = SDL_SCANCODE_X;
	game_state->keybindings[INPUT_CANCEL] = SDL_SCANCODE_Z;

	// game_state->scene = SCENE_INTRO;

	game_state->is_running = 1;

	return 1;
}

void Game_Close(GameState *game_state)
{
	// Destroy Target Texture
	SDL_DestroyTexture(game_state->target.texture);
	// SDL Window and Renderer
	SDL_DestroyRenderer(game_state->renderer);
	SDL_DestroyWindow(game_state->window);

	SDL_Quit();
}

static int InitSDL(GameState *game_state)
{
	// Variables
	SDL_Window   *window;
	SDL_Renderer *renderer;

	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		fprintf(stderr, "Error: Couldn't init. SDL. %s.\n", SDL_GetError());
		return 0;
	}

	// Window
	window = SDL_CreateWindow("C Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIN_W, WIN_H, SDL_WINDOW_SHOWN);
	if (window == NULL)
	{
		fprintf(stderr, "Error: Couldn't create window. %s.\n", SDL_GetError());
		return 0;
	}

	// Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL)
	{
		fprintf(stderr, "Error: Couldn't create renderer. %s.\n", SDL_GetError());
		SDL_DestroyWindow(window);
		return 0;
	}

	// Fill game_state
	game_state->window   = window;
	game_state->renderer = renderer;

	return 1;
}

static int InitTarget(Target *target, SDL_Renderer *renderer)
{
	// Variables
	SDL_Texture *texture;
	SDL_Rect    rect = {.w = WIN_W, .h = WIN_H};
	Uint8       scale = SCALE;

	// Target texture
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WIN_W, WIN_H);
	if (target == NULL)
	{
		fprintf(stderr, "Error: Couldn't create target texture. %s.\n", SDL_GetError());
		return 0;
	}

	// Fill Target
	target->texture = texture;
	target->rect    = rect;
	target->scale   = scale;

	return 1;
}

#include "common.h"

static void init(GameContext *game, int argc, char *argv[]);
static void close(GameContext *game);

int main(int argc, char *argv[])
{
	GameContext game = {0};

	// Init

	init(&game, argc, argv);

	Uint64 prev_time = SDL_GetPerformanceCounter();
	Uint64 curr_time;

	// Main Loop

	while(game.is_running)
	{
		// Elapsed Time
		//curr_time = SDL_GetPerformanceCounter();
		//game.elapsed_time = (double)(curr_time - prev_time) / (double)SDL_GetPerformanceFrequency();
		//prev_time = curr_time;
		//game.accumulator += game.elapsed_time;

		// Events
		Events(&game);

		// Clear Screen
		ClearScreen(&game);

		// Update
		SDL_SetRenderDrawColor(game.renderer, 255, 255, 255, 255);
		SDL_RenderDrawLine(game.renderer, 0, 0, 10, 10);

		// Draw Screen
		DrawScreen(&game);
	}

	// Close

	close(&game);
	WriteSaveFile(&game);

	printf("\n\033[1;97mSEE YOU SPACE COWBOY...\033[0m\n\n");

	return 0;
}

void init(GameContext *game, int argc, char *argv[])
{
	Uint32 flags_init   = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	Uint32 flags_window = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	Uint32 flags_render = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;

	// Load SAVE.DAT

	LoadSaveFile(game);

	// Args

	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "vsync") == 0)
		{
			game->settings.vsync = true;
			SDL_Log("VSync Enabled\n");
		}
		else if (strcmp(argv[i], "fullscreen") == 0)
		{
			game->settings.fullscreen = true;
			SDL_Log("Fullscreen Enabled\n");
		}
	}

	// Apply Settings

	if (game->settings.scale < 1)
		game->settings.scale = 1;

	if (game->settings.angle < 0 || game->settings.angle > 3)
		game->settings.angle = 0;

	if (game->settings.vsync)
		flags_render |= SDL_RENDERER_PRESENTVSYNC;

	if (game->settings.fullscreen)
		flags_window |= SDL_WINDOW_FULLSCREEN_DESKTOP;

	// Init

	int w = (game->settings.angle % 2) == 0 ? WINDOW_W : WINDOW_H;
	int h = (game->settings.angle % 2) == 0 ? WINDOW_H : WINDOW_W;

	SDL_SetHint(SDL_HINT_AUDIODRIVER, "alsa");
	ASSERT(SDL_Init(flags_init) == 0, SDL_GetError());

	game->window = SDL_CreateWindow("C Invaders",
			SDL_WINDOWPOS_UNDEFINED,
			SDL_WINDOWPOS_UNDEFINED,
			w * game->settings.scale,
			h * game->settings.scale,
			flags_window);
	ASSERT(game->window != NULL, SDL_GetError());

	game->renderer = SDL_CreateRenderer(game->window, -1, flags_render);
	ASSERT(game->renderer != NULL, SDL_GetError());

	game->fbuffer = SDL_CreateTexture(game->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_W, WINDOW_H);
	ASSERT(game->fbuffer != NULL, SDL_GetError());

	SDL_SetWindowMinimumSize(game->window,
			w * game->settings.scale,
			h * game->settings.scale);

	game->is_running = true;
}

void close(GameContext *game)
{
	SDL_DestroyTexture(game->fbuffer);
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);

	SDL_Quit();
}

#include "common.h"

int main(int argc, char *argv[])
{
	GameContext game = {0};

	Uint32 init_flags   = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	Uint32 window_flags = SDL_WINDOW_SHOWN;
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;

	const double fixed_dt = 1.0f / FPS;
	Uint64 curr_time = 0, prev_time = 0;

	SDL_Event event;

	// Command Line Arguments
	for(int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "vsync") == 0)
		{
			render_flags |= SDL_RENDERER_PRESENTVSYNC;
			SDL_Log("VSync Enabled\n");
		}
	}

	// Init SDL
	ASSERT(SDL_Init(init_flags) == 0, SDL_GetError());

	game.window = SDL_CreateWindow("C Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, window_flags);
	ASSERT(game.window != NULL, SDL_GetError());

	game.renderer = SDL_CreateRenderer(game.window, -1, render_flags);
	ASSERT(game.renderer != NULL, SDL_GetError());

	game.buffer = SDL_CreateTexture(game.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_W, WINDOW_H);
	ASSERT(game.buffer != NULL, SDL_GetError());

	SDL_SetRenderDrawBlendMode(game.renderer, SDL_BLENDMODE_BLEND);
	SDL_SetTextureBlendMode(game.buffer, SDL_BLENDMODE_BLEND);
	SDL_SetWindowMinimumSize(game.window, WINDOW_W, WINDOW_H);

	// Init SDL_mixer
	ASSERT(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == 0, Mix_GetError());
	Mix_Volume(-1, 10);

	// Init Graphics
	InitGraphics(game.renderer);

	// Game Struct
	//game.scene      = SetScenePlay();
	game.scene      = SetSceneTitle();
	game.highscore  = 57300;
	game.is_running = true;

	// Main Loop
	prev_time = SDL_GetPerformanceCounter();

	while(game.is_running)
	{
		// Elapsed Time
		curr_time = SDL_GetPerformanceCounter();
		game.elapsed_time = (double)(curr_time - prev_time) / (double)(SDL_GetPerformanceFrequency());
		prev_time = curr_time;
		game.accumulator += game.elapsed_time;

		// SDL2 Events
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				game.is_running = false;
		}

		UpdateInputState();

		// Clear Screen
		SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 0);
		SDL_RenderClear(game.renderer);
		SDL_SetRenderTarget(game.renderer, game.buffer);
		SDL_RenderClear(game.renderer);

		if (game.scene.is_starting)
		{
			game.scene.init(&game);
			game.scene.is_starting = false;
		}

		game.scene.update(&game);

		for(; game.accumulator > fixed_dt; game.accumulator -= fixed_dt)
		{
			game.scene.fixed_update(&game);
			ClearInputState();
		}

		game.scene.draw(&game);

		UpdateTransition(&game);

		// Present to Screen
		SDL_SetRenderTarget(game.renderer, NULL);
		SDL_RenderCopy(game.renderer, game.buffer, NULL, NULL);
		SDL_RenderPresent(game.renderer);
	}

	CloseGraphics();

	// Close SDL_mixer
	Mix_CloseAudio();
	Mix_Quit();

	// Close SDL
	SDL_DestroyTexture(game.buffer);
	SDL_DestroyRenderer(game.renderer);
	SDL_DestroyWindow(game.window);
	SDL_Quit();

	printf("\033[97;1mSEE YOU SPACE COWBOY...\033[0m\n");
	return 0;
}


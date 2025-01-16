#include "common.h"
#include "assets.h"
#include "events.h"
#include "input.h"

static void Init(GameContext *game)
{
	SDL_Window   *window;
	SDL_Renderer *renderer;
//	SDL_Texture  *target;

	const u32 init_flags   = SDL_INIT_VIDEO | SDL_INIT_AUDIO;
	const u32 window_flags = SDL_WINDOW_SHOWN /*| SDL_WINDOW_RESIZABLE*/;
	const u32 window_pos   = SDL_WINDOWPOS_UNDEFINED;
	const u32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;

	///// Init SDL2

	ASSERT_MSG(SDL_Init(init_flags) == 0, SDL_GetError());
	ASSERT_MSG(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == 0, Mix_GetError());

	window = SDL_CreateWindow("C Invaders", window_pos, window_pos, WINDOW_W, WINDOW_H, window_flags);
	ASSERT_MSG(window != NULL, SDL_GetError());

	renderer = SDL_CreateRenderer(window, -1, render_flags);
	ASSERT_MSG(renderer != NULL, SDL_GetError());

//	target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_W, WINDOW_H);
//	ASSERT_MSG(target != NULL, SDL_GetError());

	SDL_SetWindowMinimumSize(window, WINDOW_W, WINDOW_H);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	///// Assets

	InitAssets(renderer);

	///// Game Context

	SDL_memset(game, 0, sizeof(GameContext));

	// SDL
	game->window   = window;
	game->renderer = renderer;
//	game->target   = target;

	// Input
	BindKeyToButton(SDLK_UP    , BUTTON_UP    );
	BindKeyToButton(SDLK_DOWN  , BUTTON_DOWN  );
	BindKeyToButton(SDLK_LEFT  , BUTTON_LEFT  );
	BindKeyToButton(SDLK_RIGHT , BUTTON_RIGHT );
	BindKeyToButton(SDLK_SPACE , BUTTON_FIRE  );
	BindKeyToButton(SDLK_ESCAPE, BUTTON_CANCEL);

	// Score
	SDL_strlcpy(game->score_name[0], "HIGHWIND", 9);
	SDL_strlcpy(game->score_name[1], "LEEX    ", 9);
	SDL_strlcpy(game->score_name[2], "ZEUS    ", 9);
	SDL_strlcpy(game->score_name[3], "ROLMAN  ", 9);
	SDL_strlcpy(game->score_name[4], "ARTEMIO ", 9);
	SDL_strlcpy(game->score_name[5], "ASHER   ", 9);
	SDL_strlcpy(game->score_name[6], "MASTRETA", 9);
	SDL_strlcpy(game->score_name[7], "LECHUGA ", 9);
	SDL_strlcpy(game->score_name[8], "OPHELIA ", 9);
	SDL_strlcpy(game->score_name[9], "SCOREVG ", 9);
	game->score[0] = 57300;
	game->score[1] = 37500;
	game->score[2] = 30000;
	game->score[3] = 25000;
	game->score[4] = 20000;
	game->score[5] = 17000;
	game->score[6] = 15000;
	game->score[7] = 13000;
	game->score[8] = 10000;
	game->score[9] = 5000;

	// Control
	game->is_running      = true;
	game->fixed_framerate = 120.0f;
	game->accumulator     = 0.0f;
	game->current_screen  = ScreenTitle;
}

static void Close(GameContext *game)
{
	///// Close SDL2

	Mix_CloseAudio();

//	SDL_DestroyTexture(game->target);
	SDL_DestroyRenderer(game->renderer);
	SDL_DestroyWindow(game->window);

	SDL_Quit();
}

/***** Main *****/

int main(int argc, char *argv[])
{
	GameContext game;

	Init(&game);

	while(game.is_running)
	{
		///// Clear Screen
		SDL_SetRenderDrawColor(game.renderer, 0, 0, 0, 0);
		SDL_RenderClear(game.renderer);

		///// Process Elapsed Time, SDL Events and Input
		ProcessEvents(&game);

		///// Update current screen
		game.current_screen(&game);

		///// Draw to Screen
		SDL_RenderPresent(game.renderer);
	}

	Close(&game);

	SDL_Log("\r     \n\x1B[97;1mSEE YOU SPACE COWBOY...\x1B[0m\n\n");

	return 0;
}


//void Init(GameContext *game)
//{

///// ...

//
//	InitAssets(renderer);
//
//	///// Game Context
//	game->is_running = true;
//	game->screen     = SCREEN_TITLE;
//
//	game->sdl.window   = window;
//	game->sdl.renderer = renderer;
//	game->sdl.target   = target;
//
//	game->settings.fullscreen = false;
//	game->settings.vsync      = false;
//	game->settings.show_fps   = true;
//	game->settings.scale      = 2;
//	game->settings.volume     = 20; // Range from 0 to 100
//	game->settings.score      = 0;
//	game->settings.hi_score   = 37500; // Old Konami's default hi-score
//
//	// Load settings and overwrite default settings
//	LoadFile(&game->settings);
//
//	// Apply Settings
//	SetTargetScale(game, game->settings.scale);
//	SDL_SetWindowFullscreen(game->sdl.window, game->settings.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
//	SDL_RenderSetVSync(game->sdl.renderer, game->settings.vsync);
//	Mix_MasterVolume(game->settings.volume);
//}
//
//void Close(GameContext *game)
//{
//	FreeAssets();
//
//	Mix_CloseAudio();
//
//	SDL_DestroyTexture(game->sdl.target);
//	SDL_DestroyRenderer(game->sdl.renderer);
//	SDL_DestroyWindow(game->sdl.window);
//	SDL_Quit();
//}
//

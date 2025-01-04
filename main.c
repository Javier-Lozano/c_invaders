#include "SDL.h"
#include "SDL_mixer.h"
#include "common.h"
#include "assets.h"
#include "events.h"
#include "input.h"

#include "screen_title.c"
#include "screen_config.c"

static void Init(GameContext *ctx);
static void Close(GameContext *ctx);

/***** Main *****/

int main(int argc, char *argv[])
{
	GameContext ctx;

	Init(&ctx);

	while(ctx.is_running)
	{
		switch(ctx.screen)
		{
			case SCREEN_TITLE:
				ScreenTitle(&ctx);
				break;
			case SCREEN_CONFIG:
				ScreenConfig(&ctx);
				break;
			case SCREEN_PLAY:
				ctx.is_running = false;
				break;
		}
	}

	Close(&ctx);

	printf("\n\x1B[97;1mSEE YOU SPACE COWBOY...\x1B[0m\n\n");

	return 0;
}

void Init(GameContext *ctx)
{
	SDL_memset(ctx, 0, sizeof(GameContext));

	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *target;

	const u32 init_flags   = SDL_INIT_VIDEO | SDL_INIT_AUDIO; 
	const u32 window_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;
	const u32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;

	///// Init SDL2

	ASSERT(SDL_Init(init_flags) == 0, SDL_GetError());
	ASSERT(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == 0, Mix_GetError());

	window = SDL_CreateWindow("C Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_W, WINDOW_H, window_flags);
	ASSERT(window != NULL, SDL_GetError());
	SDL_SetWindowMinimumSize(ctx->sdl.window, WINDOW_W, WINDOW_H);

	renderer = SDL_CreateRenderer(window, -1, render_flags);
	ASSERT(renderer != NULL, SDL_GetError());
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	target = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, WINDOW_W, WINDOW_H);
	ASSERT(target != NULL, SDL_GetError());

	InitAssets(renderer);

	///// Game Context
	ctx->is_running = true;
	ctx->screen     = SCREEN_TITLE;

	ctx->sdl.window   = window;
	ctx->sdl.renderer = renderer;
	ctx->sdl.target   = target;

	ctx->settings.fullscreen = false;
	ctx->settings.vsync      = false;
	ctx->settings.show_fps   = true;
	ctx->settings.scale      = 2;
	ctx->settings.volume     = 20; // Range from 0 to 100
	ctx->settings.score      = 0;
	ctx->settings.hi_score   = 37500; // Old Konami's default hi-score

	// Load settings and overwrite default settings
	LoadFile(&ctx->settings);

	// Apply Settings
	SetTargetScale(ctx, ctx->settings.scale);
	SDL_SetWindowFullscreen(ctx->sdl.window, ctx->settings.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
	SDL_RenderSetVSync(ctx->sdl.renderer, ctx->settings.vsync);
	Mix_MasterVolume(ctx->settings.volume);
}

void Close(GameContext *ctx)
{
	FreeAssets();

	Mix_CloseAudio();

	SDL_DestroyTexture(ctx->sdl.target);
	SDL_DestroyRenderer(ctx->sdl.renderer);
	SDL_DestroyWindow(ctx->sdl.window);
	SDL_Quit();
}


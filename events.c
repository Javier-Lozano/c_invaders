#include "events.h"
#include "assets.h"
#include "input.h"

static f64 g_TimeDelta;
static SDL_Rect g_TargetRect = {.w = WINDOW_W, .h = WINDOW_H};

static void ProcessTimeDelta();

void ProcessEvents(GameContext *ctx)
{
	// Process SDL2 events, Input and calculate Time Delta.
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			// Would you be kind to exit the program?
			ctx->is_running = false;
		}
		else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
		{
			// Automatically scale and center the Rendering Target
			// if the window is being Re-sized or Fullscreen mode
			// has been enabled.
			// The scaling is done by multiplying base resolution
			// by an integer factor

			int w = event.window.data1; 
			int h = event.window.data2;
			int scale = h / WINDOW_H;

			scale = scale < 1 ? 1 : scale;

			g_TargetRect.w = WINDOW_W * scale;
			g_TargetRect.h = WINDOW_H * scale;
			g_TargetRect.x = (w / 2) - (g_TargetRect.w / 2);
			g_TargetRect.y = (h / 2) - (g_TargetRect.h / 2);

			ctx->settings.scale = scale;
		}
	}

	ProcessInput();
	ProcessTimeDelta();
}

void PrepareTarget(SDLContext *sdl)
{
	// Clear Screen
	SDL_SetRenderDrawColor(sdl->renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(sdl->renderer);

	// Clear target
	//SDL_SetRenderDrawColor(sdl->renderer, 0x20, 0x20, 0, 0xFF);
	SDL_SetRenderTarget(sdl->renderer, sdl->target);
	SDL_RenderClear(sdl->renderer);
}

void RenderTarget(SDLContext *sdl, Settings *settings)
{
	f64 dt = 1.0f / GetTimeDelta();

	if (settings->show_fps)
		DrawText(sdl->renderer, "FPS:%f", 0, 0, 0xFF0000FF, dt);

	// Draw Border
	SDL_SetRenderDrawColor(sdl->renderer, 0xFF, 0, 0, 0x40);
	SDL_RenderDrawRect(sdl->renderer, NULL);

	// Draw Target
	SDL_SetRenderTarget(sdl->renderer, NULL);
	SDL_RenderCopy(sdl->renderer, sdl->target, NULL, &g_TargetRect);

	SDL_RenderPresent(sdl->renderer);
}

static void ProcessTimeDelta()
{
	static f64 past;
	static f64 present;
	const  f64 freq = (f64)SDL_GetPerformanceFrequency();

	if (past == 0)
	{
		past = SDL_GetPerformanceCounter();
	}

	present = SDL_GetPerformanceCounter();
	g_TimeDelta = (f64)(present - past) / freq;
	past = present;
}

f64 GetTimeDelta()
{
	return g_TimeDelta;
}

void SetTargetScale(GameContext *ctx, int scale)
{
	// Set Rendering Target to a desired scale if posible, the
	// window automatically resizes to fit the target while also
	// getting centered.
	// The so scale is clamped between 1 and a max value
	// (Display's Height / WINDOW_H (because the game is vertical))
	// to always fit the display.
	// This function is called when the user sets the scale manually
	// on the CONFIG screen.

	SDL_Rect display;
	int display_index = SDL_GetWindowDisplayIndex(ctx->sdl.window);
	SDL_GetDisplayBounds(display_index, &display);

	int max_scale = display.h / WINDOW_H;

	scale = CLAMP(scale, 1, max_scale);

	g_TargetRect.w = WINDOW_W * scale;
	g_TargetRect.h = WINDOW_H * scale;
	g_TargetRect.x = 0;
	g_TargetRect.y = 0;

	SDL_SetWindowSize(ctx->sdl.window, g_TargetRect.w, g_TargetRect.h);
	SDL_SetWindowPosition(ctx->sdl.window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

	ctx->settings.scale = scale;
}

void LoadFile(Settings *settings)
{
	// Load "setting.dat" file containing settings,
	// score and hi-score data. If file doesn't
	// exists create it with default data.

	SDL_RWops *file = SDL_RWFromFile("settings.dat", "r");
	if (file == NULL)
	{
		fprintf(stderr, "%s.\nCreating file.\n", SDL_GetError());
		SaveFile(settings);
		return;
	}

	SDL_RWread(file, settings, sizeof(Settings), 1);

	SDL_RWclose(file);
}

void SaveFile(Settings *settings)
{
	// Save the current state of a "Settings" context
	// overwriting any existing "settings.dat"

	SDL_RWops *file = SDL_RWFromFile("settings.dat", "w");
	if (file == NULL)
	{
		fprintf(stderr, "%s.\n", SDL_GetError());
		return;
	}

	SDL_RWwrite(file, settings, sizeof(Settings), 1);

	SDL_RWclose(file);
}


#include "system.h"

bool Game_SDL_Start()
{
	// Init. SDL
	if (SDL_Init(SDL_INIT_FLAGS) < 0)
	{
		fprintf(stderr, "Error: Couldn't init. SDL. %s\n", SDL_GetError());
		return false;
	}

	// Create Window
	g_Window = SDL_CreateWindow("C Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_W, GAME_H, SDL_WINDOW_FLAGS);
	if (g_Window == NULL)
	{
		fprintf(stderr, "Error: Couldn't create Window. %s\n", SDL_GetError());
		SDL_Quit();
		return false;
	}

	// Create Renderer
	g_Renderer = SDL_CreateRenderer(g_Window, -1, SDL_RENDER_FLAGS);
	if (g_Window == NULL)
	{
		fprintf(stderr, "Error: Couldn't create Renderer. %s\n", SDL_GetError());

		SDL_DestroyWindow(g_Window);
		SDL_Quit();

		return false;
	}

	// Create Texture as rendering target
	g_Target = SDL_CreateTexture(g_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, GAME_W, GAME_H);
	if (g_Target == NULL)
	{
		fprintf(stderr, "Error: Couldn't create Texture. %s\n", SDL_GetError());

		SDL_DestroyWindow(g_Window);
		SDL_DestroyRenderer(g_Renderer);
		SDL_Quit();

		return false;
	}

	// Source and Target Rects
	g_TargetRectSrc	= (SDL_Rect) {0, 0, GAME_W, GAME_H};
	g_TargetRectDst	= g_TargetRectSrc;

	return true;
}

void Game_SDL_End()
{
	SDL_DestroyTexture(g_Target);
	SDL_DestroyRenderer(g_Renderer);
	SDL_DestroyWindow(g_Window);
	SDL_Quit();
}

void Game_RenderPrepare()
{
	// Clear Renderer 
	SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0, 0, 0xFF);
	SDL_RenderClear(g_Renderer);

	// Set Target
	SDL_SetRenderTarget(g_Renderer, g_Target);

	// Clear Target
	SDL_SetRenderDrawColor(g_Renderer, 0, 0, 0, 0);
	SDL_RenderClear(g_Renderer);
}

void Game_Render()
{
	// Reset Render Target
	SDL_SetRenderTarget(g_Renderer, NULL);

	// Render Copy
	SDL_RenderCopy(g_Renderer, g_Target, &g_TargetRectSrc, &g_TargetRectDst);

	// Render Present
	SDL_RenderPresent(g_Renderer);
}

void Game_ScaleTarget(int s)
{
	int scale = g_Scale + s;
	// Get Display Index
	int display_index = SDL_GetWindowDisplayIndex(g_Window);
	if (display_index < 0)
	{
		printf("Error: Couldn't get display index. %s\n", SDL_GetError());
		return;
	}

	// Retrieve Display Mode
	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(display_index, &mode);

	//  Check if the Scaled Target fits the screen
	if ((GAME_W * scale) < mode.w && (GAME_H * scale) < mode.h && scale > 0)
	{
		// Update global variable
		g_Scale = scale;

		// Scale Target
		g_TargetRectDst.w = GAME_W * g_Scale;
		g_TargetRectDst.h = GAME_H * g_Scale;

		// Change Window Size
		SDL_SetWindowSize(g_Window, g_TargetRectDst.w, g_TargetRectDst.h);

		// Check if SDL_WINDOW_FULLSCREEN_DESKTOP flag is set
		if (SDL_WINDOW_FULLSCREEN_DESKTOP & SDL_GetWindowFlags(g_Window))
		{
			// Center Target
			g_TargetRectDst.x = (mode.w - g_TargetRectDst.w) / 2;
			g_TargetRectDst.y = (mode.h - g_TargetRectDst.h) / 2;
		}
		else
		{
			// Center Window
			SDL_SetWindowPosition(g_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
	}
}

void Game_ToggleFullscreen()
{
	// Check if SDL_WINDOW_FULLSCREEN_DESKTOP flag is set
	if (SDL_WINDOW_FULLSCREEN_DESKTOP & SDL_GetWindowFlags(g_Window))
	{
		// Toggle Fullscreen (From Fullscreen Mode to Window Mode)
		SDL_SetWindowFullscreen(g_Window, 0);

		// Set Target origin to 0,0
		g_TargetRectDst.x = 0;
		g_TargetRectDst.y = 0;

		// Center Window
		SDL_SetWindowPosition(g_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	else
	{
		// Get Display Index
		int display_index = SDL_GetWindowDisplayIndex(g_Window);
		if (display_index < 0)
		{
			printf("Error: Couldn't get display index. %s\n", SDL_GetError());
			return;
		}

		// Retrieve Display Mode
		SDL_DisplayMode mode;
		SDL_GetCurrentDisplayMode(display_index, &mode);

		// Toggle Fullscreen (From Window Mode to Fullscreen MOde)
		SDL_SetWindowFullscreen(g_Window, SDL_WINDOW_FULLSCREEN_DESKTOP);

		// Center Target
		g_TargetRectDst.x = (mode.w - g_TargetRectDst.w) / 2;
		g_TargetRectDst.y = (mode.h - g_TargetRectDst.h) / 2;
	}
}

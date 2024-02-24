#include "rendering.h"

///// Globals

SDL_Renderer	*g_Renderer;
int				g_Scale;

static SDL_Window	*g_Window;
static SDL_Texture	*g_Target;
static SDL_Rect		g_TargetRect;

/////

bool Rendering_Init()
{
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
	if (g_Renderer == NULL)
	{
		fprintf(stderr, "Error: Couldn't create Renderer. %s\n", SDL_GetError());

		SDL_DestroyWindow(g_Window);
		SDL_Quit();

		return false;
	}

	// Create Texture as rendering g_Target
	g_Target = SDL_CreateTexture(g_Renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, GAME_W, GAME_H);
	if (g_Target == NULL)
	{
		fprintf(stderr, "Error: Couldn't create Texture. %s\n", SDL_GetError());

		SDL_DestroyWindow(g_Window);
		SDL_DestroyRenderer(g_Renderer);
		SDL_Quit();

		return false;
	}

	// Target Rect
	g_TargetRect = (SDL_Rect) {0, 0, GAME_W, GAME_H};

	return true;
}

void Rendering_End()
{
	SDL_DestroyTexture(g_Target);
	SDL_DestroyRenderer(g_Renderer);
	SDL_DestroyWindow(g_Window);
}

void Rendering_Prepare()
{
	// Clear Renderer 
	SDL_SetRenderDrawColor(g_Renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(g_Renderer);

	// Set Target
	SDL_SetRenderTarget(g_Renderer, g_Target);

	// Clear Target
	SDL_SetRenderDrawColor(g_Renderer, 0, 0, 0, 0);
	SDL_RenderClear(g_Renderer);
}

void Rendering_Present()
{
	// Reset Render Target
	SDL_SetRenderTarget(g_Renderer, NULL);

	// Draw a border around Target in Fullscreen
	if (SDL_WINDOW_FULLSCREEN_DESKTOP & SDL_GetWindowFlags(g_Window))
	{
		SDL_Rect border = {g_TargetRect.x - 1, g_TargetRect.y - 1, g_TargetRect.w + 2, g_TargetRect.h + 2};
		SDL_SetRenderDrawColor(g_Renderer, 0x40, 0x40, 0x40, 0x40);
		SDL_RenderDrawRect(g_Renderer, &border);
	}
	// Render Copy
	SDL_RenderCopy(g_Renderer, g_Target, NULL, &g_TargetRect);

	// Render Present
	SDL_RenderPresent(g_Renderer);
}

void Rendering_SetScale(int scale)
{
	// Get Display Index
	int display_index = SDL_GetWindowDisplayIndex(g_Window);
	SDL_DisplayMode mode;

	if (display_index < 0)
	{
		printf("Error: Couldn't get display index. %s\n", SDL_GetError());
		return;
	}

	// Retrieve Display Mode
	SDL_GetCurrentDisplayMode(display_index, &mode);

	//  Check if the Scaled Target fits the screen
	if (scale > 0 && (GAME_W * scale) < mode.w && (GAME_H * scale) < mode.h)
	{
		// Scale Target
		g_TargetRect.w = GAME_W * scale;
		g_TargetRect.h = GAME_H * scale;

		// Change Window Size
		SDL_SetWindowSize(g_Window, g_TargetRect.w, g_TargetRect.h);

		// Check if SDL_WINDOW_FULLSCREEN_DESKTOP flag is set
		if (SDL_WINDOW_FULLSCREEN_DESKTOP & SDL_GetWindowFlags(g_Window))
		{
			// Center Target
			g_TargetRect.x = (mode.w - g_TargetRect.w) / 2;
			g_TargetRect.y = (mode.h - g_TargetRect.h) / 2;
		}
		else
		{
			// Center Window
			SDL_SetWindowPosition(g_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
		}
	}
}

void Rendering_ToggleFullscreen()
{
	// Check if SDL_WINDOW_FULLSCREEN_DESKTOP flag is set
	if (SDL_WINDOW_FULLSCREEN_DESKTOP & SDL_GetWindowFlags(g_Window))
	{
		// Toggle Fullscreen (From Fullscreen Mode to Window Mode)
		SDL_SetWindowFullscreen(g_Window, 0);

		// Set Target origin to 0,0
		g_TargetRect.x = 0;
		g_TargetRect.y = 0;

		// Center Window
		SDL_SetWindowPosition(g_Window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}
	else
	{
		// Get Display Index
		int display_index = SDL_GetWindowDisplayIndex(g_Window);
		SDL_DisplayMode mode;

		if (display_index < 0)
		{
			printf("Error: Couldn't get display index. %s\n", SDL_GetError());
			return;
		}

		// Retrieve Display Mode
		SDL_GetCurrentDisplayMode(display_index, &mode);

		// Toggle Fullscreen (From Window Mode to Fullscreen MOde)
		SDL_SetWindowFullscreen(g_Window, SDL_WINDOW_FULLSCREEN_DESKTOP);

		// Center Target
		g_TargetRect.x = (mode.w - g_TargetRect.w) / 2;
		g_TargetRect.y = (mode.h - g_TargetRect.h) / 2;
	}
}

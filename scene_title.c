#include "scene.h"

void Game_SceneTitle()
{
	// Variables
	int pivot = 0;
	bool loop = true;
	SDL_Rect test = {0,0,32,32};

	while(loop)
	{
		// Event
		while(SDL_PollEvent(&g_Event))
		{
			if (g_Event.type == SDL_QUIT)
			{
				g_MainLoop = false;
				loop = false;
			}

			if (g_Event.type == SDL_KEYDOWN)
			{
				if (g_Event.key.keysym.scancode == SDL_SCANCODE_UP)
				{
					pivot--;
				}
				if (g_Event.key.keysym.scancode == SDL_SCANCODE_DOWN)
				{
					pivot++;
				}
			}
		}

		// Prepare Rendering
		Game_RenderPrepare();
		Game_Background();
		// Render Rect
		SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0, 0xFF, 0xFF);
		SDL_RenderDrawRect(g_Renderer, &test);

		// Render Game
		Game_Render();
	}
}


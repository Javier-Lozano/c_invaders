#include "scene.h"

///// Title Screen

void Scene_Title()
{
	// Variables
	SDL_Event event;
	int pivot = 0;
	bool loop = true;
	SDL_Rect test = {0,0,32,32};

	while(loop)
	{
		// Event
		while(SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
			{
				loop = false;
				g_GameScene = SCENE_EXIT;
			}

			if (event.type == SDL_KEYDOWN)
			{
				if (event.key.keysym.scancode == SDL_SCANCODE_UP)
				{
					pivot--;
				}
				if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)
				{
					pivot++;
				}
			}
		}

		// Prepare Rendering
		Rendering_Prepare();
	
		// Draw Background
		Scene_Background();

		// Render Rect
		SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0, 0xFF, 0xFF);
		SDL_RenderDrawRect(g_Renderer, &test);

		SDL_Rect texture_rect = {(GAME_W - 184) / 2, 32, 184, 112};
		SDL_RenderCopy(g_Renderer, g_Resources.textures[TEXTURE_TITLE], NULL, &texture_rect);

		// Render Game
		Rendering_Present();
	}
}


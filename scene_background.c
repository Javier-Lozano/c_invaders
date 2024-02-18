#include "scene.h"

// A Starfield Background
void Game_Background()
{
	// Variables
	static SDL_Point star[BG_STARS];
	static SDL_Color color[BG_STARS];
	static bool do_once = true;

	// Initialize Stars
	if (do_once)
	{
		for(int i = 0; i < BG_STARS; i++)
		{
			color[i].r = (rand() % 0xA0) + 0x7F;
			color[i].g = (rand() % 0x80) + 0x60;
			color[i].b = (rand() % 0x60) + 0x40;
			star[i].x = rand() % GAME_W;
			star[i].y = rand() % GAME_H;
		}
		do_once = false;
	}

	for(int i = 0; i < BG_STARS; i++)
	{
		if (i < 50)
		{
			star[i].y += 1;
		}
		else if (i >= 50 && i < 100)
		{
			star[i].y += 2;
		}
		else if (i >= 100)
		{
			star[i].y += 4;
		}

		if (star[i].y > GAME_H)
		{
			star[i].y -= GAME_H;
		}

		SDL_SetRenderDrawColor(g_Renderer, color[i].r, color[i].g, color[i].b, 0xFF);
		SDL_RenderDrawPoint(g_Renderer, star[i].x, star[i].y);
	}
}

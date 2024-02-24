#include "scene.h"

///// Macros

#define STARS 150

///// Globals

static SDL_Color g_Colors[] = {
	{0xFF, 0xFF, 0xFF, 0xFF},
	{0xFF, 0, 0, 0xFF},
	{0, 0xFF, 0, 0xFF},
	{0, 0, 0xFF, 0xFF},
	{0xFF, 0xFF, 0, 0xFF},
	{0xFF, 0, 0xFF, 0xFF},
	{0, 0xFF, 0xFF, 0xFF}
};

///// Starfield Background

void Scene_Background()
{
	// Variables
	static bool do_once = true;
	static SDL_Point star[STARS];
	static float angle[STARS];

	// Init. Stars
	if (do_once)
	{
		for(int i = 0; i < STARS; i++)
		{
			star[i].x = rand() % GAME_W;
			star[i].y = rand() % GAME_H;
			angle[i] = (rand() % 314) / 100.0f;
		}
		do_once = false;
	}

	// Update all stars
	for(int i = 0; i < STARS; i++)
	{
		// Get color
		SDL_Color color = g_Colors[(i + 1) % 7];

		// Get brigthness value
		float value = SDL_sin(angle[i]);

		// Move angle
		angle[i] += 0.03f;
		if (angle[i] > 3.14) { angle[i] -= 3.14f; }

		// First half moves slow
		if (i < (STARS / 2))
		{
			star[i].y += 1;
			value *= 0x90;
		}
		// Second Hald moves faster
		else
		{
			star[i].y += 2;
			value *= 0xB8;
		}

		// Once a Star reached bottom move it back to the top
		if (star[i].y > GAME_H)
		{
			star[i].y -= GAME_H;
		}

		// Set brightness
		if (color.r != 0) { color.r = (int) value; }
		if (color.g != 0) { color.g = (int) value; }
		if (color.b != 0) { color.b = (int) value; }

		SDL_SetRenderDrawColor(g_Renderer, color.r, color.g, color.b, color.a);
		SDL_RenderDrawPoint(g_Renderer, star[i].x, star[i].y);
	}
}

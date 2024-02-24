#include "resources.h"

///// Globals

Resources g_Resources;

extern SDL_Renderer *g_Renderer;

static bool LoadTexture(const char *path, SDL_Texture **texture_ptr);

bool Resources_Init()
{
	int success;
	// Load Textures
	g_Resources.textures = (SDL_Texture**)malloc(sizeof(SDL_Texture*) * TEXTURE_COUNT);

	success = LoadTexture("resources/title.bmp", &g_Resources.textures[TEXTURE_TITLE]);

	return success;
}

void Resources_End()
{
	SDL_DestroyTexture(g_Resources.textures[TEXTURE_TITLE]);
	free(g_Resources.textures);
}

static bool LoadTexture(const char *path, SDL_Texture **texture_ptr)
{
	// Variables
	SDL_Surface *surface	= NULL;

	// Load Title Screen
	surface = SDL_LoadBMP(path);
	if (surface == NULL)
	{
		fprintf(stderr, "Coulnd't load surface. %s\n", SDL_GetError());
		return false;
	}

	// Check if Image has an Alpha channel
	if (SDL_ISPIXELFORMAT_ALPHA(surface->format->format))
	{
		// Color Key Background
		// Color Key is defined as 0xFF00FF as in 0xRRGGBB
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(
					surface->format,
					(COLOR_KEY >> 16)	& 0xFF,
					(COLOR_KEY >> 8)	& 0xFF,
					COLOR_KEY			& 0xFF
					));
	}

	// Create Texture from Surface
	*texture_ptr = SDL_CreateTextureFromSurface(g_Renderer, surface);
	if (*texture_ptr == NULL)
	{
		fprintf(stderr, "Coulnd't create texture. %s\n", SDL_GetError());
		return false;
	}

	// Free Surface
	SDL_FreeSurface(surface);

	return true;
}

#include "resources.h"

///// Globals

SDL_Color g_Colors[] = {
	{0xFF, 0xFF, 0xFF, 0xFF},	// WHITE
	{0xFF, 0, 0, 0xFF},			// RED
	{0, 0xFF, 0, 0xFF},			// GREEN
	{0, 0, 0xFF, 0xFF},			// BLUE
	{0xFF, 0xFF, 0, 0xFF},		// YELLOW
	{0xFF, 0, 0xFF, 0xFF},		// MAGENTA
	{0, 0xFF, 0xFF, 0xFF}		// CYAN
};

Resources g_Resources;


extern SDL_Renderer *g_Renderer;

static bool LoadTexture(const char *path, SDL_Texture **texture_ptr);

bool Resources_Init()
{
	// Load Textures
	g_Resources.textures = (SDL_Texture**)calloc(TEXTURE_COUNT, sizeof(SDL_Texture*));

	LoadTexture("resources/title.bmp", &g_Resources.textures[TEXTURE_TITLE]);
	LoadTexture("resources/font.bmp", &g_Resources.textures[TEXTURE_FONT]);

	for(int i = 0; i < 2; i++)
	{
		if (g_Resources.textures[i] == NULL) { return false; }
	}

	// Create Font
	g_Resources.font = FontCreate(g_Resources.textures[TEXTURE_FONT], " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ÁÉÍÓÚÑ♠♥♦♣", 8, 8);
	if (g_Resources.font == NULL) { return false; }

	return true;
}

void Resources_End()
{
	// Destroy Textures
	if (g_Resources.textures != NULL)
	{
		for(int i = 0; i < 2; i++)
		{
			if (g_Resources.textures[i] != NULL)
			{
				SDL_DestroyTexture(g_Resources.textures[i]);
			}
		}
		free(g_Resources.textures);
	}

	// Destroy Font
	FontDestroy(g_Resources.font);
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

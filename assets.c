#include "assets.h"

/***** Globals *****/

static SDL_Texture *g_SpriteSheetTexture;
static SDL_Surface *g_SpriteSheetSurface;
//static SDL_Surface *g_Shields;
//static int g_ShieldTilemap [] = {
//};

/***** Functions *****/

int InitAssets()
{
	SDL_Surface *surface;
	SDL_Texture *texture;

	surface = SDL_LoadBMP(SPRITESHEET_PATH);
	if (surface == NULL)
		goto assets_init_error;

	texture = SDL_CreateTextureFromSurface(GetRenderer(), surface);
	if (texture == NULL)
		goto assets_init_error;

	g_SpriteSheetTexture = texture;
	g_SpriteSheetSurface = surface;

	return 1;

assets_init_error:
	printf("Couldn't initialize assets. %s\n", SDL_GetError());
	return 0;
}

void CloseAssets()
{
	SDL_DestroyTexture(g_SpriteSheetTexture);
	SDL_FreeSurface(g_SpriteSheetSurface);
//	SDL_FreeSurface(g_Shields);
}

void DrawSprite(int index, int x, int y)
{
	if (index < 0 || index > 0x7F)
		return;

	SDL_Rect source = { 
		.x = (index % 16) * SPRITE_SIZE,
		.y = (index / 16) * SPRITE_SIZE,
		.w = SPRITE_SIZE,
		.h = SPRITE_SIZE
	};
	SDL_Rect target = {
		.x = x,
		.y = y,
		.w = SPRITE_SIZE,
		.h = SPRITE_SIZE
	};

	SDL_RenderCopy(GetRenderer(), g_SpriteSheetTexture, &source, &target);
}

void DrawSpriteColor(int index, int x, int y, Uint32 color)
{
	SDL_SetTextureColorMod(g_SpriteSheetTexture,
			(color >> 24) & 0xFF,
			(color >> 16) & 0xFF,
			(color >>  8) & 0xFF);
	SDL_SetTextureAlphaMod(g_SpriteSheetTexture, color & 0xFF);

	DrawSprite(index, x, y);

	SDL_SetTextureAlphaMod(g_SpriteSheetTexture, 0xFF);
	SDL_SetTextureColorMod(g_SpriteSheetTexture, 0xFF, 0xFF, 0xFF);
}

int DrawText(const char *str, int x, int y, Uint32 color, ...)
{
	char temp[256];
	va_list args;
	int xx = x;
	int yy = y;
	int count = 0;

	SDL_SetTextureColorMod(g_SpriteSheetTexture,
			(color >> 24) & 0xFF,
			(color >> 16) & 0xFF,
			(color >>  8) & 0xFF);
	SDL_SetTextureAlphaMod(g_SpriteSheetTexture, color & 0xFF);

	va_start(args, color);
	for(int i = 0; str[i]; i++)
	{
		// Character is ASCII
		if (str[i] >= 0 && str[i] <= 0x7F)
		{
			// '\n' (New Line)
			if (str[i] == 0x0A)
			{
				xx = x;
				yy += SPRITE_SIZE;
				count++;
				continue;
			}
			// ' ' (Space)
			if (str[i] == 0x20)
			{
				xx += SPRITE_SIZE;
				count++;
				continue;
			}

			// Parse Format
			if (str[i] == '%')
			{
				i++;
				if (str[i] == '\0')
					break;

				if (str[i] == 'd' || str[i] == 'X' || str[i] == 'f' || str[i] == 'c' || str[i] == 's')
				{
					vsnprintf(temp, 256, (char[]){'%', str[i], '\0'}, args);
					int c = DrawText(temp, xx, yy, color);
					xx += c * SPRITE_SIZE;
					count += c;
				}
				continue;
			}

			int tile = str[i];

			// Lowercase to Uppercase
			if (tile >= 0x61 && tile <= 0x7A) 
				tile -= 0x20;

			tile -= 0x20;
			DrawSprite(tile, xx, yy);
			xx += SPRITE_SIZE;
			count++;
		}
	}
	va_end(args);

	SDL_SetTextureAlphaMod(g_SpriteSheetTexture, 0xFF);
	SDL_SetTextureColorMod(g_SpriteSheetTexture, 0xFF, 0xFF, 0xFF);

	return count;
}


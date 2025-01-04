#include "SDL.h"
#include "SDL_mixer.h"
#include "assets.h"
#include "common.h"

#define SPRITESHEET_PATH "assets/spritesheet.bmp"

static SDL_Texture *g_SpriteSheetTexture;
static SDL_Surface *g_SpriteSheetSurface;
static Mix_Chunk   *g_SFX[SFX_COUNT];

void InitAssets(SDL_Renderer *renderer)
{
	SDL_Surface *surface;
	SDL_Texture *texture;
	Mix_Chunk   *sound[SFX_COUNT];

	surface = SDL_LoadBMP(SPRITESHEET_PATH);
	ASSERT(surface, "Couldn't load spritesheet.");

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));

	texture = SDL_CreateTextureFromSurface(renderer, surface);
	ASSERT(texture, "Couldn't create texture.");

	sound[0] = Mix_LoadWAV("assets/14_item2.wav");
	sound[1] = Mix_LoadWAV("assets/09_select1.wav");
	sound[2] = Mix_LoadWAV("assets/80_chaindone.wav");
	for(int i = 0; i < SFX_COUNT; i++)
	{
		ASSERT(sound[i], "Couldn't load sound.");
	}

	g_SpriteSheetTexture = texture;
	g_SpriteSheetSurface = surface;
	SDL_memcpy(g_SFX, sound, sizeof(Mix_Chunk) * SFX_COUNT);
}

void FreeAssets()
{
	SDL_DestroyTexture(g_SpriteSheetTexture);
	SDL_FreeSurface(g_SpriteSheetSurface);
//	SDL_FreeSurface(g_Shields);
}

void DrawSprite(SDL_Renderer *renderer, int index, int x, int y)
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

	SDL_RenderCopy(renderer, g_SpriteSheetTexture, &source, &target);
}

void DrawSpriteColor(SDL_Renderer *renderer, int index, int x, int y, u32 color)
{
	SDL_Color c;
	SDL_GetTextureColorMod(g_SpriteSheetTexture, &c.r, &c.g, &c.b);
	SDL_GetTextureAlphaMod(g_SpriteSheetTexture, &c.a);

	SDL_SetTextureColorMod(g_SpriteSheetTexture,
			(color >> 24) & 0xFF,
			(color >> 16) & 0xFF,
			(color >>  8) & 0xFF);
	SDL_SetTextureAlphaMod(g_SpriteSheetTexture, color & 0xFF);

	DrawSprite(renderer, index, x, y);

	SDL_SetTextureAlphaMod(g_SpriteSheetTexture, c.a);
	SDL_SetTextureColorMod(g_SpriteSheetTexture, c.r, c.g, c.b);
}

int DrawText(SDL_Renderer *renderer, const char *str, int x, int y, u32 color, ...)
{
	char temp[256];
	va_list args;
	int xx = x;
	int yy = y;
	int count = 0;

	SDL_Color c;
	SDL_GetTextureColorMod(g_SpriteSheetTexture, &c.r, &c.g, &c.b);
	SDL_GetTextureAlphaMod(g_SpriteSheetTexture, &c.a);

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
					if (str[i] == 'f')
						vsnprintf(temp, 256, (char[]){'%', '.', '2', str[i], '\0'}, args);
					else
						vsnprintf(temp, 256, (char[]){'%', str[i], '\0'}, args);
					int c = DrawText(renderer, temp, xx, yy, color);
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
			DrawSprite(renderer, tile, xx, yy);
			xx += SPRITE_SIZE;
			count++;
		}
	}
	va_end(args);

	SDL_SetTextureAlphaMod(g_SpriteSheetTexture, c.a);
	SDL_SetTextureColorMod(g_SpriteSheetTexture, c.r, c.g, c.b);

	return count;
}

void PlaySound(int index)
{
	if (index < 0 || index > SFX_COUNT)
		return;

	Mix_PlayChannel(-1, g_SFX[index], 0);
}


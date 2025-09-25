#include "common.h"

#define GRAPHICS_WIDTH  (128)
#define GRAPHICS_HEIGHT (128)

#define MAX_FRAMES (8)

#define GLYPH_SIZE  (8)

static const SDL_Rect g_Sprites[SPR_COUNT] = {
		{   0,  96, 16, 8 }, /* SPR_PLAYER */
		{  16,  96, 16, 8 }, /* SPR_ENEMY_A_1 */
		{  32,  96, 16, 8 }, /* SPR_ENEMY_A_2 */
		{  48,  96, 16, 8 }, /* SPR_ENEMY_B_1 */
		{  64,  96, 16, 8 }, /* SPR_ENEMY_B_2 */
		{  80,  96, 16, 8 }, /* SPR_ENEMY_C_1 */
		{  96,  96, 16, 8 }, /* SPR_ENEMY_C_2 */
		{ 112,  96, 16, 8 }, /* SPR_ENEMY_UFO */
		{   0, 104, 16, 8 }, /* SPR_PLAYER_DEAD_1 */
		{  16, 104, 16, 8 }, /* SPR_PLAYER_DEAD_2 */
		{  32, 104, 16, 8 }, /* SPR_PLAYER_DEAD_3 */
		{  48, 104, 16, 8 }, /* SPR_PLAYER_DEAD_4 */
		{  64, 104,  8, 8 }, /* SPR_HIT_1 */
		{  72, 104,  8, 8 }, /* SPR_HIT_2 */
		{  80, 104,  8, 8 }, /* SPR_HIT_3 */
		{  88, 104,  8, 8 }, /* SPR_HIT_4 */
		{  96, 104,  8, 8 }, /* SPR_BULLET_HIT_1 */
		{ 104, 104,  8, 8 }, /* SPR_BULLET_HIT_2 */
		{ 112, 104,  8, 8 }, /* SPR_BULLET_HIT_3 */
		{ 120, 104,  8, 8 }, /* SPR_BULLET_HIT_4 */
		{   0, 112,  8, 8 }, /* SPR_BULLET_PLAYER */
		{   8, 112,  8, 8 }, /* SPR_BULLET_A_1 */
		{  16, 112,  8, 8 }, /* SPR_BULLET_A_2 */
		{  24, 112,  8, 8 }, /* SPR_BULLET_A_3 */
		{  32, 112,  8, 8 }, /* SPR_BULLET_A_4 */
		{  40, 112,  8, 8 }, /* SPR_BULLET_B_1 */
		{  48, 112,  8, 8 }, /* SPR_BULLET_B_2 */
		{  56, 112,  8, 8 }, /* SPR_BULLET_B_3 */
		{  64, 112,  8, 8 }, /* SPR_BULLET_B_4 */
		{  72, 112,  8, 8 }, /* SPR_BULLET_C_1 */
		{  80, 112,  8, 8 }, /* SPR_BULLET_C_2 */
		{  88, 112,  8, 8 }, /* SPR_BULLET_C_3 */
		{  96, 112,  8, 8 }, /* SPR_BULLET_C_4 */
		{ 104, 112,  8, 8 }, /* SPR_SHIELD_1 */
		{ 112, 112,  8, 8 }, /* SPR_SHIELD_2 */
		{ 120, 112,  8, 8 }, /* SPR_SHIELD_3 */
		{   0, 120,  8, 8 }, /* SPR_TILE_1 */
		{   8, 120,  8, 8 }, /* SPR_TILE_2 */
		{  16, 120,  8, 8 }, /* SPR_TILE_3 */
		{  24, 120,  8, 8 }, /* SPR_TILE_4 */
		{  32, 120,  8, 8 }, /* SPR_TILE_5 */
		{  40, 120,  8, 8 }, /* SPR_TILE_6 */
		{  48, 120,  8, 8 }, /* SPR_TILE_7 */
		{  56, 120,  8, 8 }, /* SPR_TILE_8 */
		{  64, 120,  8, 8 }, /* SPR_TILE_9 */
		{  72, 120,  8, 8 }, /* SPR_TILE_10 */
		{  80, 120,  8, 8 }, /* SPR_TILE_11 */
		{  88, 120,  8, 8 }, /* SPR_TILE_12 */
		{  96, 120,  8, 8 }, /* SPR_TILE_13 */
		{ 104, 120,  8, 8 }  /* SPR_CURSOR */
};

typedef struct {
	int frames[MAX_FRAMES];
	int length;
} Sequence;

static const Sequence g_Sequences[SEQ_COUNT] = {
	/* SEQ_PLAYER_DEAD */
	{
		.frames = { SPR_PLAYER_DEAD_1, SPR_PLAYER_DEAD_2, SPR_PLAYER_DEAD_3, SPR_PLAYER_DEAD_4 },
		.length = 4
	},
	/* SEQ_HIT */
	{
		.frames = { SPR_HIT_1, SPR_HIT_2, SPR_HIT_3, SPR_HIT_4, -1 },
		.length = 5
	},
	/* SEQ_BULLET_HIT */
	{
		.frames = { SPR_BULLET_HIT_1, SPR_BULLET_HIT_2, SPR_BULLET_HIT_3, SPR_BULLET_HIT_4, -1},
		.length = 4
	},
	/* SEQ_BULLET_A */
	{
		.frames = { SPR_BULLET_A_1, SPR_BULLET_A_2, SPR_BULLET_A_3, SPR_BULLET_A_4 },
		.length = 4
	},
	/* SEQ_BULLET_B */
	{
		.frames = { SPR_BULLET_B_1, SPR_BULLET_B_2, SPR_BULLET_B_3, SPR_BULLET_B_4 },
		.length = 4
	},
	/* SEQ_BULLET_C */
	{
		.frames = { SPR_BULLET_C_1, SPR_BULLET_C_2, SPR_BULLET_C_3, SPR_BULLET_C_4 },
		.length = 4
	},
};

static const unsigned int g_UTF8Map[12] = {
	0xC381, // Á
	0xC389, // É
	0xC38D, // Í
	0xC393, // Ó
	0xC39A, // Ú
	0xC391, // Ñ
	0xC3A1, // á
	0xC3A9, // é
	0xC3AD, // í
	0xC3B3, // ó
	0xC3BA, // ú
	0xC3B1  // ñ
};

static SDL_Texture *g_GraphicsTexture;

void InitGraphics(SDL_Renderer *renderer)
{
	SDL_Surface *surface = SDL_LoadBMP("assets/sprites.bmp");
	ASSERT(surface != NULL, SDL_GetError());

	surface->format->palette->colors[1] = (SDL_Color) {255, 255, 255, 255};
	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0));

	g_GraphicsTexture = SDL_CreateTextureFromSurface(renderer, surface);
	ASSERT(g_GraphicsTexture != NULL, SDL_GetError());

	SDL_SetTextureBlendMode(g_GraphicsTexture, SDL_BLENDMODE_BLEND);

	SDL_FreeSurface(surface);
}

void CloseGraphics()
{
	SDL_DestroyTexture(g_GraphicsTexture);
}

void SetGraphicsColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	SDL_SetTextureColorMod(g_GraphicsTexture, r, g, b);
	SDL_SetTextureAlphaMod(g_GraphicsTexture, a);
}

void DrawTile(SDL_Renderer *renderer, int tile, int x, int y)
{
	if (tile < 0 || tile > 255)
		return;

	SDL_Rect dst = {x, y, GLYPH_SIZE, GLYPH_SIZE};
	SDL_Rect src = {
		src.x = (tile % 16) * GLYPH_SIZE,
		src.y = (tile / 16) * GLYPH_SIZE,
		GLYPH_SIZE,
		GLYPH_SIZE
	};

	SDL_RenderCopy(renderer, g_GraphicsTexture, &src, &dst);
}

void DrawTileRGBA(SDL_Renderer *renderer, int tile, int x, int y, unsigned int rgba)
{
	SetGraphicsColor(
			(rgba & 0xFF000000) >> 24,
			(rgba & 0x00FF0000) >> 16,
			(rgba & 0x0000FF00) >> 8,
			(rgba & 0x000000FF)
			);
	DrawTile(renderer, tile, x, y);
}

void DrawSprite(SDL_Renderer *renderer, SpriteID sprite, int x, int y)
{
	if (sprite < 0 || sprite > SPR_COUNT-1)
		return;

	SDL_Rect dst = { x, y, g_Sprites[sprite].w, g_Sprites[sprite].h };
	SDL_RenderCopy(renderer, g_GraphicsTexture, &g_Sprites[sprite], &dst);
}

void DrawSpriteRGBA(SDL_Renderer *renderer, SpriteID sprite, int x, int y, unsigned int rgba)
{
	SetGraphicsColor(
			(rgba & 0xFF000000) >> 24,
			(rgba & 0x00FF0000) >> 16,
			(rgba & 0x0000FF00) >> 8,
			(rgba & 0x000000FF)
			);
	DrawSprite(renderer, sprite, x, y);
}

void InitAnimation(Animation *anim, SequenceID seq_id, float seconds)
{
	anim->seq_id   = seq_id;
	anim->pivot    = 0;
	anim->duration = seconds;
	anim->timer    = 0.0f;
}

void PlayAnimation(SDL_Renderer *renderer, Animation *anim, float dt, int x, int y)
{
	if (anim->seq_id < 0 || anim->seq_id > SEQ_COUNT-1)
		return;

	const Sequence *seq = &g_Sequences[anim->seq_id];
	const float step = anim->duration / seq->length;

	anim->timer += dt;
	if (anim->timer >= step)
	{
		anim->timer -= step;
		anim->pivot++;

		if (anim->pivot == seq->length)
			anim->pivot = 0;
	}

	DrawSprite(renderer, seq->frames[anim->pivot], x, y);
}

static int draw_text(SDL_Renderer *renderer, const char *text, bool formatted, int *x, int *y, va_list args)
{
	int start_x = *x;
	int count   = 0;

	for(int i = 0; text[i]; i++)
	{
		unsigned char c = (unsigned char)text[i];

		if (c == ' ')
		{
			*x += GLYPH_SIZE;
			count++;
			continue;
		}
		else if (c == '\n')
		{
			*x = start_x;
			*y += GLYPH_SIZE;
			count++;
			continue;
		}
		else if (formatted && c == '%')
		{
			char format[8] = {0};
			char temp[256] = {0};
			char f = text[i+1];

			//TODO: Implement floating point precision format specifier

			switch(f)
			{
				case '\0':
					continue;
				case 'd':
				case 'x':
				case 'X':
				case 'f':
				case 'c':
				case 's':
					snprintf(format, 8, "%%%c", f);
					vsnprintf(temp, 256, format, args);
					count += draw_text(renderer, temp, false, x, y, args);
					i++;
					continue;
				default:
					break;
			}
		}
		else if (c == '^')
		{
			unsigned char r = 0, g = 0, b = 0;
			char *endptr = NULL;
			int code = strtol(&text[i+1], &endptr, 10);

			if (code > 0 && code <= 8)
			{
				r = (code & 1) ? 255 : 0;
				g = (code & 2) ? 255 : 0;
				b = (code & 4) ? 255 : 0;
				SDL_SetTextureColorMod(g_GraphicsTexture, r, g, b);

				int offset = (endptr - (text + i)) - 1;
				i += offset;
				continue;
			}
		}

		if (c < 0x80)
		{
			DrawTile(renderer, c, *x, *y);
			*x += GLYPH_SIZE;
			count++;
		}
		else if (c > 0xC1 && c < 0xF5)
		{
			const unsigned char *str = (unsigned char *)text;
			unsigned int utf8 = (unsigned int)c;

			int index = i;

			while((str[++index] & 0xC0) == 0x80)
				utf8 = (utf8 << 8) | (unsigned int)str[index];

			i = index - 1;

			for(int u = 0; u < 12; u++)
			{
				if (utf8 == g_UTF8Map[u])
				{
					DrawTile(renderer, 128+u, *x, *y);
					*x += GLYPH_SIZE;
					count++;
					break;
				}
			}
		}
	}

	return count;
}

int DrawText(SDL_Renderer *renderer, const char *str, int x, int y, ...)
{
	int count;
	va_list args;

	va_start(args, y);
	count = draw_text(renderer, str, true, &x, &y, args);
	va_end(args);

	return count;
}

int DrawTextRGBA(SDL_Renderer *renderer, const char *str, int x, int y, unsigned int rgba, ...)
{
	int count;
	va_list args;

	SetGraphicsColor(
			(rgba & 0xFF000000) >> 24,
			(rgba & 0x00FF0000) >> 16,
			(rgba & 0x0000FF00) >> 8,
			(rgba & 0x000000FF)
			);

	va_start(args, rgba);
	count = draw_text(renderer, str, true, &x, &y, args);
	va_end(args);

	return count;
}

void DrawGraphicsTexture(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, g_GraphicsTexture, NULL, &(SDL_Rect){0,0,GRAPHICS_WIDTH,GRAPHICS_HEIGHT});
}


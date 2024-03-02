#ifndef _BITMAP_FONT_H_
#define _BITMAP_FONT_H_

#include <SDL2/SDL.h>

#define TEMP_STR_LEN 256

///// Data Structures

typedef struct {
	Uint32 *map;
	size_t mapSize;
	SDL_Point glyphSize;
	SDL_Texture *texture;
} BitmapFont;

///// Functions

BitmapFont *FontCreate(SDL_Texture *texture, const char *char_map, int glyph_w, int glyph_h);
void FontDestroy(BitmapFont *font);
int FontPrint(SDL_Renderer *renderer, BitmapFont *font, const char *str, int x, int y, SDL_Color *color, ...);

#endif // _BITMAP_FONT_H_


#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "font.h"

///// Macros

#define COLOR_KEY 0xFF00FF

///// Data Structures

typedef struct {
	SDL_Texture **textures;
	BitmapFont *font;
} Resources;

enum {
	C_WHITE,
	C_RED,
	C_GREEN,
	C_BLUE,
	C_YELLOW,
	C_MAGENTA,
	C_CYAN,
	C_COUNT
};

enum {
	TEXTURE_TITLE,
	TEXTURE_FONT,
	TEXTURE_CHARA,
	TEXTURE_COUNT
};

///// Functions

bool Resources_Init();
void Resources_End();

#endif // _RESOURCES_H_


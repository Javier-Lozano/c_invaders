#ifndef _RESOURCES_H_
#define _RESOURCES_H_

#include <stdbool.h>
#include <SDL2/SDL.h>

///// Macros

#define COLOR_KEY 0xFF00FF

///// Data Structures

typedef struct {
	SDL_Texture **textures;
} Resources;

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


#ifndef ASSETS_H_
#define ASSETS_H_

#include "SDL.h"
#include "common_types.h"

#define SPRITE_SIZE  (8)

// Sprite Indexes

// SFX Indexes

enum {
	SFX_CURSOR,
	SFX_SELECT,
	SFX_DONE,
	SFX_COUNT
};

void InitAssets(SDL_Renderer *renderer);
void FreeAssets();

void DrawSprite(SDL_Renderer *renderer, int index, int x, int y);
void DrawSpriteColor(SDL_Renderer *renderer, int index, int x, int y, u32 color);
int DrawText(SDL_Renderer *renderer, const char *str, int x, int y, u32 color, ...);

void PlaySound(int index);

#endif // ASSETS_H_

#ifndef ASSETS_H_
#define ASSETS_H_

#include "SDL.h"
#include "rendering.h"

/***** MACROS *****/ 

#define SPRITESHEET_PATH "assets/spritesheet2.bmp"
#define SPRITE_SIZE  (8)

/***** Functions *****/

int InitAssets();
void CloseAssets();

void DrawSprite(int index, int x, int y);
void DrawSpriteColor(int index, int x, int y, Uint32 color);
int DrawText(const char *str, int x, int y, Uint32 color, ...);

#endif // ASSETS_H_

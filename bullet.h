#ifndef _BULLET_H_
#define _BULLET_H_

#include <SDL2/SDL.h>
#include "defines.h"

// Data Structs

typedef struct  {
	SDL_Rect box;
	SDL_Point speed;
	int isActive;
} Bullet;

// Functions

Bullet InitBullet(int x, int y, int x_speed, int y_speed);
void UpdateBullets(Bullet *bullets);
void RenderBullets(SDL_Renderer *renderer, Bullet *bullets);

#endif // _BULLET_H_

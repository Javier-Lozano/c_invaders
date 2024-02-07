#ifndef _BULLET_H_
#define _BULLET_H_

#include <SDL2/SDL.h>
#include "defines.h"

// Data Structs

typedef struct {
	SDL_Rect box;
	int speed;
} Bullet;

// Functions

void SpawnBullet(Bullet *bullet, int x, int y, int speed);
void UpdateBullets(Bullet *bullets);
void RenderBullets(SDL_Renderer *renderer, Bullet *bullets);

#endif // _BULLET_H_

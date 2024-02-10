#ifndef _BULLET_H_
#define _BULLET_H_

#include "defines.h"
#include "entity.h"

// Globals

extern int g_BulletCount;

// Functions

void SpawnBullet(Entity *bullet, int x, int y, int speed);
void UpdateBullets(Entity *bullets);
void RenderBullets(SDL_Renderer *renderer, Entity *bullets);

#endif // _BULLET_H_

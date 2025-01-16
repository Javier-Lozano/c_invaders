#ifndef BULLET_H_
#define BULLET_H_

#include "common_types.h"

#define BULLET_COUNT (56)

typedef struct bullet_st{
	bool is_active;
	f32 x, y, w, h;
	f32 speed;
} Bullet;

void InitBullet(Bullet *bullet, int x, int y, f32 speed);
void UpdateBullet(Bullet bullets[], f64 dt);
void DrawBullet(SDL_Renderer *renderer, Bullet bullets[]);

#endif // BULLET_H_


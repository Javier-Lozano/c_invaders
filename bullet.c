#include "common.h"
#include "bullet.h"

void InitBullet(Bullet *bullet, int x, int y, f32 speed)
{
	bullet->is_active = true;
	bullet->x = x;
	bullet->y = y;
	bullet->w = 3;
	bullet->h = 8;
	bullet->speed = speed;
}

void UpdateBullet(Bullet bullet[], f64 dt)
{
	for(int i = 0; i < BULLET_COUNT; i++)
	{
		if (bullet[i].is_active)
		{
			bullet[i].y += bullet[i].speed * dt * 60;

			if (bullet[i].y < 0 || bullet[i].y > WINDOW_H)
			{
				bullet[i].is_active = false;
			}
		}
	}
}

void DrawBullet(SDL_Renderer *renderer, Bullet bullet[])
{
	for(int i = 0; i < BULLET_COUNT; i++)
	{
		SDL_SetRenderDrawColor(renderer, 0xFF,0,0,0xFF);
		SDL_RenderDrawRect(renderer, &(SDL_Rect){
				bullet[i].x,
				bullet[i].y,
				bullet[i].w,
				bullet[i].h,
				});
	}
}

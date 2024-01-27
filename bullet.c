#include "bullet.h"

Bullet InitBullet(int x, int y, int x_speed, int y_speed)
{
	Bullet b = {
		{ x, y, 2, 8},
		{ x_speed, y_speed },
		1
	};

	return b;
}

void UpdateBullets(Bullet *bullets)
{
	for(int i = 0; i < BULLET_MAX; i++)
	{
		if (bullets[i].isActive)
		{
			bullets[i].box.x += bullets[i].speed.x;
			bullets[i].box.y += bullets[i].speed.y;

			if (bullets[i].box.x < 0 || bullets[i].box.x > GAME_W || bullets[i].box.y < 16 || bullets[i].box.y > GAME_H - 16)
			{
				bullets[i].isActive = 0;
			}
		}
		
	}
}

void RenderBullets(SDL_Renderer *renderer, Bullet *bullets)
{
	for(int i = 0; i < BULLET_MAX; i++)
	{
		if (bullets[i].isActive)
		{
			// Render Collision Box
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderDrawRect(renderer, &bullets[i].box);
		}
		
	}
}

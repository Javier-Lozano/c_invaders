#include "bullet.h"

void SpawnBullet(Bullet *bullet, int x, int y, int speed)
{
	// Set bullet initial values
	bullet->box.x = x;
	bullet->box.y = y;
	bullet->box.w = BULLET_W;
	bullet->box.h = BULLET_H;
	bullet->speed = speed;
}

void UpdateBullets(Bullet *bullets)
{
	// Iterate all bullets
	for(int i = 0; i < BULLET_COUNT; i++)
	{
		// If a bullet was shot it's speed wont be 0
		if (bullets[i].speed != 0)
		{
			// Move bullet
			bullets[i].box.y += bullets[i].speed;

			// Check if bullet reached game borders
			if (CHECK_BORDERS(bullets[i].box))
			{
				// Set speed to 0
				bullets[i].speed = 0;
			}

			// Player bullet collision test
			//if (i == 0)
			//{
			//	for(int j= 1; j < BULLET_COUNT; j++)
			//	{

			//		if (bullets[j].speed != 0 && CHECK_COLLISION(bullets[0].box, bullets[j].box))
			//		{
			//			printf("Bullet 0 Collided!\n");
			//			bullets[0].speed = 0;
			//			bullets[j].speed = 0;
			//		}
			//	}
			//}
		}
	}
}

void RenderBullets(SDL_Renderer *renderer, Bullet *bullets)
{
	// Render Collision Box
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	for(int i = 0; i < BULLET_COUNT; i++)
	{
		if (bullets[i].speed != 0)
		{
			SDL_RenderDrawRect(renderer, &bullets[i].box);
		}
	}
}

#include "bullet.h"

void SpawnBullet(Entity *bullet, int x, int y, int speed)
{
	// Init. Bullet
	*bullet = (Entity) {
		{x, y , BULLET_W, BULLET_H},
		speed,
		ENTITY_PLAY,
		0
	};
	g_BulletCount++;
}

void UpdateBullets(Entity *bullets)
{
	// Iterate over all bullets
	for(int i = 0; i < g_BulletCount; i++)
	{
		//Check if a bullet was fired by checking it's speed
		if (bullets[i].state == ENTITY_PLAY)
		{
			// Move bullet
			bullets[i].box.y += bullets[i].speed;

			// Check if bullet reached game borders
			if (CHECK_BORDERS(bullets[i].box))
			{
				// Change State
				bullets[i].state = ENTITY_END;
			}
		}

		if (bullets[i].state == ENTITY_END)
		{
			// Wait 1 seconds before disapearing
			if (++bullets[i].waitTime == 10)
			{
				bullets[i].state = ENTITY_NONE;
			}
		}
	}
}


void RenderBullets(SDL_Renderer *renderer, Entity *bullets)
{
	// Render Collision Box
	for(int i = 0; i < g_BulletCount; i++)
	{
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		if (bullets[i].state == ENTITY_PLAY)
		{
			SDL_RenderDrawRect(renderer, &bullets[i].box);
		}

		SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
		if (bullets[i].state == ENTITY_END)
		{
			SDL_RenderDrawRect(renderer, &bullets[i].box);
		}
	}
}

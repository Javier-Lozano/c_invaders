#include "c_invaders.h"
#include "scene_play.h"

/***** Main *****/

int main (int argc, char *argv[])
{
	// Variables
	Entity entity[ID_COUNT] = {0};
	int stage = 0;
	int lives = 3;
	int remaining = 55;

	// Init Stage
	InitStage(entity, stage);

	if (Game_Init())
	{
		while(g_MainLoop)
		{
			/***** Events *****/

			Game_HandleEvents();

			/***** Update *****/

			if (lives > 0)
			{
				UpdatePlayer(&entity[ID_PLAYER], &entity[ID_BULLET_P]);

				remaining = UpdateEnemies(&entity[ID_ENEMY], &entity[ID_BULLET_E], &entity[ID_UFO], stage);

				ProcessCollisions(entity);
			}

			if (remaining == 0)
			{
				stage++;
				InitStage(entity, stage);
			}

			/***** Render *****/

			RenderStage(entity);
		}
	}

	Game_Close();
	return 0;
}

void InitStage(Entity *entity, int stage)
{
	// Variables
	Entity *player = &entity[ID_PLAYER];
	Entity *enemy  = &entity[ID_ENEMY];
	Entity *bullet = &entity[ID_BULLET_P];
	Entity *ufo    = &entity[ID_UFO];

	// Player
	player->box.w = 8;
	player->box.h = 8;
	player->box.x = 112;
	player->box.y = 272;
	player->type = TYPE_PLAYER;
	player->state = STATE_ALIVE;

	// Player Bullet
	for(int i = 0; i < ENEMY_COUNT; i++)
	{
		int col = (i % 11);
		int row = (i / 11);

		int start_y = 144 + (stage * 16);
		if (start_y > 240) { start_y = 240; }

		enemy[i].box.w = 8;
		enemy[i].box.h = 8;
		enemy[i].box.x = (col * 16) + 32;
		enemy[i].box.y = (row * -16) + start_y;
		switch(row)
		{
			case 0:
			case 1:
				enemy[i].type = TYPE_ENEMY_LARGE;
				break;
			case 2:
			case 3:
				enemy[i].type = TYPE_ENEMY_MEDIUM;
				break;
			case 4:
				enemy[i].type = TYPE_ENEMY_SMALL;
				break;
		}
		enemy[i].state = STATE_ALIVE;
	}

	// Bullets
	for(int i = 0; i < BULLET_COUNT; i++)
	{
		bullet[i].box.w = 2;
		bullet[i].box.h = 8;
		bullet[i].type = TYPE_BULLET;
	}

	// UFO
	ufo->box.w = 8;
	ufo->box.h = 8;
	ufo->box.y = 40;
	ufo->type = TYPE_UFO;
	ufo->wait = (rand() % 5) + 5;
}

void UpdatePlayer(Entity *player, Entity *player_bullet)
{
	// Move Player
	player->box.x += ((double)CHECK_HELD(INPUT_RIGHT) - (double)CHECK_HELD(INPUT_LEFT)) * 60.0f * g_DeltaTime;
	if (player->box.x > WIN_W - 8) { player->box.x = WIN_W - 8; }
	if (player->box.x < 0) { player->box.x = 0; }

	// Fire Bullet
	if (CHECK_PRESSED(INPUT_FIRE) && !CHECK_ALIVE(player_bullet->state))
	{
		player_bullet->state |= STATE_ALIVE;
		player_bullet->box.x = player->box.x + 3;
		player_bullet->box.y = player->box.y;
	}

	// Move Player Bullet
	if (CHECK_ALIVE(player_bullet->state))
	{
		player_bullet->box.y -= 4.0f * 60.0f * g_DeltaTime;

		if (player_bullet->box.y < 32)
		{
			player_bullet->state &= ~STATE_ALIVE;
		}
	}
}

int UpdateEnemies(Entity *enemy, Entity *enemy_bullets, Entity *ufo, int stage)
{
	static double enemy_timer;
	static int direction = 1;
	static int pivot;
	int first_enemy = -1;
	bool drop = false;
	int defeated = ENEMY_COUNT;

	for(int i = 0; i < ENEMY_COUNT; i++)
	{
		// Check Enemy Bullets

		// Check Enemies
		if (CHECK_ALIVE(enemy[i].state))
		{
			// Find first enemy;
			if (first_enemy < 0)
			{
				first_enemy = i;
			}

			// Check edges
			if ((direction == 1 && enemy[i].box.x + 8 >= WIN_W) || (direction == -1 && enemy[i].box.x <= 0))
			{
				drop = true;
			}
		}
	}

	// Update Enemy after a 60th of a second
	enemy_timer += g_DeltaTime;
	if (enemy_timer > 0.01666f)
	{
		// Reset Timer
		enemy_timer = 0.0f;

		while( !CHECK_ALIVE(enemy[pivot].state) )
		{
			defeated--;
			if (defeated == 0)
			{
				return defeated;
			}

			// Move pivot to next enemy
			pivot = (pivot + 1) % ENEMY_COUNT;
		}

		if (pivot == first_enemy & drop)
		{
			drop = false;
			direction *= -1;
			for(int i = 0; i < ENEMY_COUNT; i++)
			{
				enemy[i].state |= STATE_DROP;
			}
		}

		// Move enemy
		enemy[pivot].box.x += direction * 2;
		if (enemy[pivot].state & STATE_DROP)
		{
			enemy[pivot].state &= ~STATE_DROP;
			enemy[pivot].box.y += 8;
		}

		// Move pivot to next enemy
		pivot = (pivot + 1) % ENEMY_COUNT;
	}

	return defeated;
}

void ProcessCollisions(Entity *entity)
{
	// Variables
	Entity *player        = &entity[ID_PLAYER];
	Entity *player_bullet = &entity[ID_BULLET_P];
	Entity *enemy         = &entity[ID_ENEMY];
	Entity *enemy_bullets = &entity[ID_BULLET_E];
	Entity *ufo           = &entity[ID_UFO];

	// Player Bullet <-> Enemy Collision
	if (CHECK_ALIVE(player_bullet->state))
	{
		for(int i = 0; i < ENEMY_COUNT; i++)
		{
			if (CHECK_ALIVE(enemy[i].state))
			{
				if(CheckCollision(&player_bullet->box, &enemy[i].box))
				{
					enemy[i].state &= ~STATE_ALIVE;
					player_bullet->state &= ~STATE_ALIVE;
				}
			}
		}
	}
}

bool CheckCollision(SDL_FRect *a, SDL_FRect *b)
{
	return 
		(a->x < (b->x + b->w)) &&
		((a->x + a->w) > b->x) &&
		(a->y < (b->y + b->h)) &&
		((a->y + a->h) > b->y);
}

void RenderStage(Entity *entity)
{
	// Clear Screen
	SDL_SetRenderDrawColor(g_Renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(g_Renderer);

	for(int i = 0; i < ID_COUNT; i++)
	{
		if (!CHECK_ALIVE(entity[i].state))
		{
			continue;
		}

		switch(entity[i].type)
		{
			case TYPE_PLAYER:
				SDL_SetRenderDrawColor(g_Renderer, 0, 0xFF, 0, 0xFF);
				break;
			case TYPE_ENEMY_SMALL:
				SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0, 0x80, 0xFF);
				break;
			case TYPE_ENEMY_MEDIUM:
				SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0, 0x60, 0xFF);
				break;
			case TYPE_ENEMY_LARGE:
				SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0, 0x40, 0xFF);
				break;
			case TYPE_BULLET:
				SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0x80, 0, 0xFF);
				break;
			case TYPE_UFO:
				SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0, 0xFF, 0xFF);
				break;
		}

		SDL_RenderDrawRectF(g_Renderer, &entity[i].box);
	}

	SDL_RenderPresent(g_Renderer);
}


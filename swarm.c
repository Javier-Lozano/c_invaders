#include "swarm.h"

static int g_LeftmostX, g_RightmostX, g_Direction;

void InitSwarm(Swarm *swarm)
{
	// Init. all Enemies. Bottom to Top, left to Right
	for(int i = 0; i < SWARM_COUNT; i++)
	{
		// Get Rows and Columns
		int col = (i % SWARM_COLS);
		int row = (i / SWARM_COLS);

		// Get X and Y yositions
		int x = col * (ENEMY_W + ENEMY_GAP) + (((GAME_W - (SWARM_COLS * (ENEMY_W + ENEMY_GAP)))) / 2);
		int y = -row * (ENEMY_H + (ENEMY_GAP * 2)) + (GAME_H / 2);
		swarm->enemy[i] = (Enemy) {
			{x, y, ENEMY_W, ENEMY_H},
			1
		};
	}

	swarm->enemyCount = SWARM_COUNT;
	// Init Swarm direction to Right
	g_Direction = 1;
}

void UpdateEnemy(Enemy *enemy)
{
	// Check if enemy is alive
	if (CHECK_BIT_BOOL(enemy->state, ENEMY_IS_ALIVE))
	{
		// Move horizontally
		enemy->box.x += g_Direction * ENEMY_SPEED;
		if (CHECK_BIT_BOOL(enemy->state, ENEMY_DROP_ROW))
		{
			enemy->box.y += 8;
			enemy->state = UNSET_BIT(enemy->state, ENEMY_DROP_ROW);
		}
		if (enemy->box.x < g_LeftmostX) {g_LeftmostX = enemy->box.x; }
		if (enemy->box.x + enemy->box.w > g_RightmostX) { g_RightmostX = enemy->box.x + enemy->box.w; }
	}
}

void UpdateSwarm(Swarm *swarm)
{
	// Index
	static int index;

	// Update Enemy
	UpdateEnemy(&swarm->enemy[index]);

	// At the end of a loop
	if (index++ == swarm->enemyCount - 1)
	{
		g_LeftmostX += g_Direction * ENEMY_SPEED;
		g_RightmostX += g_Direction * ENEMY_SPEED;
		// Check border
		if (CHECK_X_BORDER(g_LeftmostX, g_RightmostX))
		{
			g_Direction *= -1;
			for(int i = 0; i < SWARM_COUNT; i++)
			{
				swarm->enemy[i].state = SET_BIT(swarm->enemy[i].state, ENEMY_DROP_ROW);
			}
		}
		// Reset Index
		index = 0;
		// Init. Swarm X Bounding
		g_LeftmostX = swarm->enemy[0].box.x;
		g_RightmostX = swarm->enemy[0].box.x + swarm->enemy[0].box.w;
	}
}

void RenderSwarm(SDL_Renderer *renderer, Swarm *swarm)
{
	SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
	for(int i = 0; i < SWARM_COUNT; i++)
	{
		if (CHECK_BIT_BOOL(swarm->enemy[i].state, ENEMY_IS_ALIVE))
		{
			SDL_RenderDrawRect(renderer, &swarm->enemy[i].box);
		}
	}
	SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0xFF, 0xFF);
	SDL_RenderDrawLine(renderer, g_LeftmostX, GAME_H / 2, g_RightmostX, GAME_H /2);
}

#include "enemy.h"

static int g_LX, g_RX, g_Direction, g_Index;

void InitEnemies(Entity *enemy)
{
	for(int i = ENTITY_ENEMY; i < ENTITY_UFO; i++)
	{
		// Get Rows nad Columns
		int col = ((i - 1) % ENEMY_COLS);
		int row = ((i - 1) / ENEMY_COLS);

		// Get x and y positions
		int x = col * (ENEMY_W + ENEMY_GAP) + (GAME_W - (ENEMY_COLS * (ENEMY_W + ENEMY_GAP))) / 2;
		int y = -row * (ENEMY_H + (ENEMY_GAP * 2)) + (GAME_H / 3);

		// Init. Enemy
		enemy[i] = (Entity) {
			{x, y, ENEMY_W, ENEMY_H},
			ENEMY_SPEED,
			ENTITY_PLAY,
			0
		};
	}

	//for(int i = 1; i < 55; i++)
	//{
	//	enemy[i].state = 0;
	//}

	// Init. Globals
	g_Direction = 1;
	g_Index = ENTITY_ENEMY;
	g_LX = GAME_W / 2; // Enemy group Leftmost X position
	g_RX = GAME_W / 2; // Enemy group Rightmost X position
}

void UpdateEnemies(Entity *enemy)
{
	// Check if index is at the end of the loop
	if (g_Index >= ENTITY_UFO)
	{
		// Reset Index
		g_Index = ENTITY_ENEMY;

		// Check if enemy is close to any of the side borders
		if (CHECK_X_BORDER(g_LX + (g_Direction * ENEMY_SPEED), g_RX + (g_Direction * ENEMY_SPEED)))
		{
			// Change Direction
			g_Direction *= -1;

			// Set all playing enemies to move down 1 row
			for(int i = ENTITY_ENEMY; i < ENTITY_UFO; i++)
			{
				if (enemy[i].state == ENTITY_PLAY)
				{
					enemy[i].state = ENTITY_ENEMY_DROP;
				}
			}
		}

		// Reset LX and RX
		g_LX = GAME_W / 2;
		g_RX = GAME_W / 2;

	}

	// If current index enemy is dead skip to the next alive or the last position
	for(; enemy[g_Index].state == ENTITY_NONE && g_Index < ENTITY_UFO; g_Index++);

	// Move enemy down 1 row
	if (enemy[g_Index].state == ENTITY_ENEMY_DROP)
	{
		enemy[g_Index].box.y += (ENEMY_H + ENEMY_GAP);
		enemy[g_Index].state = ENTITY_PLAY;
	}

	// If enemy is active
	if (enemy[g_Index].state == ENTITY_PLAY)
	{
		// Move enemy
		enemy[g_Index].box.x += g_Direction * ENEMY_SPEED;

		// Update bounds
		int x1 = enemy[g_Index].box.x;
		int x2 = enemy[g_Index].box.x + enemy[g_Index].box.w;

		// If enemy has reached bottom, teleport it back to the top
		if (enemy[g_Index].box.y >= PLAYER_Y)
		{
			enemy[g_Index].box.y = (GAME_H / 3);
		}

		// Update LX and RX
		if (x1 < g_LX) { g_LX = x1; }
		if (x2 > g_RX) { g_RX = x2; }

	}

	// Release: Advance to next index.
	//g_Index++;
}

void RenderEnemies(SDL_Renderer *renderer, Entity *enemy)
{
	// Debug: Draw LX and RX
	//SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0xFF, 0xFF);
	//SDL_RenderDrawLine(renderer, g_LX, GAME_H / 2, g_RX, GAME_H /2);

	// Debug: Draw Enemies and Highlight current index
	SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
	for(int i = ENTITY_ENEMY; i < ENTITY_UFO; i++)
	{
		if (enemy[i].state == ENTITY_PLAY || enemy[i].state == ENTITY_ENEMY_DROP)
		{
			//if (i == g_Index)
			//{
			//	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			//	SDL_RenderFillRect(renderer, &enemy[i].box);
			//}
			//else
			{
				SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
				SDL_RenderDrawRect(renderer, &enemy[i].box);
			}
		}
	}

	// Debug: Advance to next index.
	g_Index++;
}

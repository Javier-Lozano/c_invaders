#include "player.h"

void InitPlayer(Entity *player)
{
	// Init. Player
	*player = (Entity) {
		{PLAYER_X, PLAYER_Y, PLAYER_W, PLAYER_H},
		PLAYER_SPEED,
		ENTITY_PLAY,
		0
	};
}

void UpdatePlayer(Entity *player, Entity *bullet)
{
	if (player->state == ENTITY_PLAY) {
		// Input
		int x_dir = (CHECK_BIT(g_Input, INPUT_RIGHT)) - (CHECK_BIT(g_Input, INPUT_LEFT));
		player->box.x += (x_dir * player->speed);

		// Limit Horizontal Movement
		if (player->box.x < 0) { player->box.x = 0; }
		if (player->box.x + player->box.w > GAME_W) { player->box.x = (GAME_W - player->box.w); }

		// Fire Bullet
		if (CHECK_BIT(g_Input, INPUT_FIRE) && (bullet->state == ENTITY_NONE))
		{
			SpawnBullet(bullet, player->box.x + (player->box.w / 2), player->box.y, -6);
		}
	}
}

void RenderPlayer(SDL_Renderer *renderer, Entity *player)
{
	// Render Collision Box
	SDL_SetRenderDrawColor(renderer, 0, 0xFF ,0, 0xFF);
	SDL_RenderDrawRect(renderer, &player->box);

	// Render Sprite
}

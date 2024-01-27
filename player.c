#include "player.h"

void InitPlayer(Player *player)
{
	*player = (Player) {
		{ PLAYER_X, PLAYER_Y, PLAYER_W, PLAYER_H },
		PLAYER_SPD_X,
		0
	};
}

void UpdatePlayer(Player *player, Bullet *bullets)
{
	// Input
	int x_dir = (CHECK_BIT_BOOL(g_Input, INPUT_RIGHT)) - (CHECK_BIT_BOOL(g_Input, INPUT_LEFT));
	player->box.x += (x_dir * player->speed);

	// Limit Horizontal Movement
	if (player->box.x < 0) { player->box.x = 0; }
	if (player->box.x > (GAME_W - player->box.w)) { player->box.x = (GAME_W - player->box.w); }

	// Fire Bullets
	if (CHECK_BIT_BOOL(g_Input, INPUT_FIRE) && !bullets[PLAYER_BULLET].isActive)
	{
		bullets[PLAYER_BULLET] = InitBullet(player->box.x, player->box.y, 0, -8);
	}
}

void RenderPlayer(SDL_Renderer *renderer, Player *player)
{
	// Render Collision Box
	SDL_SetRenderDrawColor(renderer, 0, 0xFF ,0, 0xFF);
	SDL_RenderDrawRect(renderer, &player->box);

	// Render Sprite
}

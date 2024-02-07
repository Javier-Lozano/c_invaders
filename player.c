#include "player.h"

void InitPlayer(Player *player)
{
	player->box.x = (GAME_W / 2) - (PLAYER_W / 2);
	player->box.y = (GAME_H - (GAME_BORDER * 2));
	player->box.w = PLAYER_W;
	player->box.h = PLAYER_H;
}

void UpdatePlayer(Player *player)
{
	// Input
	int x_dir = (CHECK_INPUT(g_Input, INPUT_RIGHT)) - (CHECK_INPUT(g_Input, INPUT_LEFT));
	player->box.x += (x_dir * PLAYER_SPD_X);

	// Limit Horizontal Movement
	if (player->box.x < 0) { player->box.x = 0; }
	if (player->box.x > (GAME_W - player->box.w)) { player->box.x = (GAME_W - player->box.w); }

	// Fire Bullet
	if (CHECK_INPUT(g_Input, INPUT_FIRE) && !player->bullet->speed != 0)
	{
		SpawnBullet(player->bullet, player->box.x + (player->box.w / 2), player->box.y, -6);
	}
}

void RenderPlayer(SDL_Renderer *renderer, Player *player)
{
	// Render Collision Box
	SDL_SetRenderDrawColor(renderer, 0, 0xFF ,0, 0xFF);
	SDL_RenderDrawRect(renderer, &player->box);

	// Render Sprite
}

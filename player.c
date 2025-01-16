#include "common.h"
#include "player.h"
#include "assets.h"

#include "bullet.h"

void InitPlayer(Player *player, bool respawn)
{
	player->w = 14;
	player->h = 8;
	player->x = (WINDOW_W / 2) - 7;
	player->y = 272;

	if (respawn)
		player->lives -= 1;
	else
		player->lives = 3;

	player->state = PLAYER_ALIVE;
}

void UpdatePlayer(GameContext *game, Player *player, f64 dt)
{
	f32 xx = (IsKeyDown(SDLK_RIGHT) - IsKeyDown(SDLK_LEFT)) * dt * 120;
	player->x += xx;

	if (player->x < 0) { player->x = 0; }
	if (player->x > WINDOW_W - player->w) { player->x = WINDOW_W - player->w; }

	if (IsKeyPressed(SDLK_SPACE) && !player->bullet->is_active)
	{
		InitBullet(player->bullet, player->x+8, player->y, -4);
	}
}

void DrawPlayer(SDL_Renderer *renderer, GameContext *game, Player *player)
{
	// Draw Player
	DrawSpriteColor(renderer, 64, player->x, player->y, 0x00FF00FF);
	DrawSpriteColor(renderer, 65, player->x+8, player->y, 0x00FF00FF);
	SDL_SetRenderDrawColor(renderer, 0xFF, 0, 0, 0xFF);
}

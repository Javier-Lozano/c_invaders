#ifndef PLAYER_H_
#define PLAYER_H_

#include "common_types.h"

typedef struct game_context_st GameContext;
typedef struct bullet_st Bullet;

enum {
	PLAYER_ALIVE = 0,
	PLAYER_DEAD  = 1
};

typedef struct player_st {
	Bullet *bullet;
	f32 x, y, w, h;
	f32 speed;
	u8  lives;
	u8  state;
} Player;

void InitPlayer(Player *player, bool respawn);
void UpdatePlayer(GameContext *game, Player *player, f64 dt);
void DrawPlayer(SDL_Renderer *renderer, GameContext *game, Player *player);

#endif // PLAYER_H_

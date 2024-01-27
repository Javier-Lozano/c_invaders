#ifndef _PLAYER_H_
#define _PLAYER_H_

#include <SDL2/SDL.h>
#include "defines.h"
#include "input.h"
#include "bullet.h"

// Data Structs

typedef struct {
	SDL_Rect box;
	int speed;
	int state;
} Player;

// Globals

extern Uint8 g_Input;

// Functions

void InitPlayer(Player *player);
void UpdatePlayer(Player *player, Bullet *bullets);
void RenderPlayer(SDL_Renderer *renderer, Player *player);

#endif // _PLAYER_H_

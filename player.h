#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "defines.h"
#include "input.h"
#include "entity.h"
#include "bullet.h"

// Globals

extern Uint8 g_Input;

// Functions

void InitPlayer(Entity *player);
void UpdatePlayer(Entity *player, Entity *bullet);
void RenderPlayer(SDL_Renderer *renderer, Entity *player);

#endif // _PLAYER_H_

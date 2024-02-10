#ifndef _ENEMY_H_
#define _ENEMY_H_

#include <SDL2/SDL.h>
#include "defines.h"
#include "entity.h"
#include "bullet.h"

void InitEnemies(Entity *enemy);
void UpdateEnemies(Entity *enemy);
void RenderEnemies(SDL_Renderer *renderer, Entity *enemy);

#endif //_ENEMY_H_

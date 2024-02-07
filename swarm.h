#ifndef _SWARM_H_
#define _SWARM_H_

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "bullet.h"
#include "defines.h"

enum EnemyStateShifts {
	ENEMY_IS_ALIVE,
	ENEMY_DROP_ROW,
	ENEMY_FIRE
};
typedef struct {
	SDL_Rect box;
	Uint8 state;
	int waitTimer;
	int fireRate;
} Enemy;

typedef struct {
	Enemy enemy[SWARM_COUNT];
	int enemyCount;
} Swarm;

void UpdateEnemy(Enemy *enemy);
void InitSwarm(Swarm *swarm);
void UpdateSwarm(Swarm *swarm);
void RenderSwarm(SDL_Renderer *renderer, Swarm *swarm);

#endif //_SWARM_H_

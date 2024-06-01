#ifndef _SCENE_PLAY_H_
#define _SCENE_PLAY_H_

/***** Macros *****/

#define ENEMY_COUNT 55
#define BULLET_COUNT 56

#define CHECK_ALIVE(state) (state & STATE_ALIVE)

/***** Enums *****/

enum { // Entity IDs
	ID_PLAYER   = 0,
	ID_ENEMY    = 1,
	ID_BULLET_P = 56,
	ID_BULLET_E = 57,
	ID_UFO      = 112,
	ID_COUNT
};

enum { // Entity State
	STATE_ALIVE = 1,
	STATE_DROP  = 2
};

enum { // Entity Type
	TYPE_PLAYER,
	TYPE_ENEMY_LARGE,
	TYPE_ENEMY_MEDIUM,
	TYPE_ENEMY_SMALL,
	TYPE_UFO,
	TYPE_BULLET
};

enum { // Enemy Directon
	DIR_RIGHT = 1,
	DIR_LEFT  = -1
};

/***** Types *****/

typedef struct {
	SDL_FRect box;
	float wait;
	Uint8 type;
	Uint8 state;
} Entity;

/***** Functions *****/

void InitStage(Entity *entity, int stage);
void UpdatePlayer(Entity *player, Entity *player_bullet);
int  UpdateEnemies(Entity *enemy, Entity *enemy_bullets, Entity *ufo, double *timer, int *index, int *direction, int stage);
void ProcessCollisions(Entity *entity);
bool CheckCollision(SDL_FRect *a, SDL_FRect *b);
void RenderStage(Entity *entity);

#endif // _SCENE_PLAY_H_

#include "invaders.h"

#define DEBUG_ENTITY 1

#define ENTITY_COUNT (1 + 55 + 56 + 1)

///// Scene

typedef enum {
	STATE_INTRO,
	STATE_PLAY,
	STATE_GAMEOVER
} SceneState;

static SceneState g_State;

///// Entity

typedef enum {
	ENTITY_NONE,
	ENTITY_PLAYER,
	ENTITY_ALIEN,
	ENTITY_UFO,
	ENTITY_BULLET,
} EntityType;

struct Entity;

typedef struct DataPlayer {
	struct Entity *bullet;
	int dir;
	float timer;
} DataPlayer;

typedef union EntityData {
	DataPlayer player;
} EntityData;

typedef struct Entity {
	EntityType type;
	EntityData data;
	float x[2], y[2], w, h;
	int speed;
	bool is_alive;
	Animation anim;
} Entity;

static Entity g_Entity[ENTITY_COUNT];

///// Player Functions

static void init_player(Entity *e)
{
	e->type = ENTITY_PLAYER;
	e->x[0] = e->x[1] = (WINDOW_W / 2) - 6;
	e->y[0] = e->y[1] = 272;
	e->w = 12;
	e->h = 8;
	e->speed = 2;
	e->is_alive = true;
	InitAnimation(&e->anim, SEQ_PLAYER_DEAD, 0.25f);
}

static void update_player(Entity *e, bool sim_frame)
{
	DataPlayer *d = &e->data.player;

	if (g_State != STATE_PLAY)
		return;

	if (sim_frame && e->is_alive)
	{
		e->x[1] = e->x[0];
		e->x[0] += e->speed * d->dir;
		if (e->x[0] < 0)
			e->x[0] = 0;
		if (e->x[0] > WINDOW_W - 12)
			e->x[0] = WINDOW_W - 12;
	}
	else
	{
		e->data.player.dir = (int)IsKeyDown(SDLK_RIGHT) - (int)IsKeyDown(SDLK_LEFT);
	}

}

static void draw_player(SDL_Renderer *renderer, Entity *e, double dt, double alpha)
{
	float lx = LERP((float)e->x[1], (float)e->x[0], alpha);

	if (e->is_alive)
		DrawSpriteRGBA(renderer, SPR_PLAYER, lx-2, e->y[0], 0x00FF00FF);
	else
		PlayAnimation(renderer, &e->anim, dt, e->x[0]-2, e->y[0]);

#if defined DEBUG && DEBUG_ENTITY
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 128);
	//SDL_RenderDrawLine(renderer, lx, 0, lx, WINDOW_H);
	SDL_RenderDrawRect(renderer, &(SDL_Rect){lx, e->y[0], e->w, e->h});
#endif
}

///// Alien Functions

static void init_aliens(Entity *e)
{
	for(int i = 0; i < 55; i++)
	{
		Entity *alien = e+i;

		alien->type = ENTITY_ALIEN;
		//alien->data;
		alien->x[0] = (i%11) * 16;
		alien->y[0] = (i/11) * 16;
		alien->w = 12;
		alien->h = 8;
		alien->is_alive = true;
		InitAnimation(&alien->anim, SEQ_HIT, 0.5f);
	}
}

static void draw_aliens(Entity *e)
{
	for(int i = 0; i < 55; i++)
	{
		Entity *alien = e+i;

		alien->type = ENTITY_ALIEN;
		//alien->data;
		alien->x[0] = (i%11) * 16;
		alien->y[0] = (i/11) * 16;
		alien->w = 12;
		alien->h = 8;
		alien->is_alive = true;
		InitAnimation(&alien->anim, SEQ_HIT, 0.5f);
	}
}

///// UI

static void draw_ui(SDL_Renderer *renderer)
{
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, 0, 288, WINDOW_W, 288);
}

/////

static void init()
{
	g_State = STATE_INTRO;

	init_player(&g_Entity[0]);
}

static void update(GameContext *game, double dt)
{
	///// TEST START

	if (IsKeyPressed(SDLK_k))
		g_Entity[0].is_alive = false;

	///// TEST END

	for (int i = 0; i < ENTITY_COUNT; i++)
	{
		switch(g_Entity[i].type)
		{
			case ENTITY_NONE:
				break;
			case ENTITY_PLAYER:
				update_player(&g_Entity[i], false);
				break;
		}
	}
	(void)game;
	(void)dt;
}

static void fixed_update(GameContext *game, double fixed_dt)
{
	for (int i = 0; i < ENTITY_COUNT; i++)
	{
		switch(g_Entity[i].type)
		{
			case ENTITY_NONE:
				break;
			case ENTITY_PLAYER:
				update_player(&g_Entity[i], true);
				break;
		}
	}
	(void)game;
	(void)fixed_dt;
}

static void draw(GameContext *game, double dt, double alpha)
{
	draw_player(game->renderer, &g_Entity[0], dt, alpha);
	draw_ui(game->renderer);
	(void)dt;
}

Scene GetScenePlay()
{
	return (Scene){ init, update, fixed_update, draw, true };
}

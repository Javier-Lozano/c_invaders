#include "invaders.h"

///// Scene

typedef enum {
	STATE_INTRO,
	STATE_PLAY,
	STATE_GAMEOVER
} SceneState;

static SceneState g_State;

/////

static void init()
{
}

static void update(GameContext *game, double dt)
{
	(void)game;
	(void)dt;
}

static void fixed_update(GameContext *game, double fixed_dt)
{
	(void)game;
	(void)fixed_dt;
}

static void draw(GameContext *game, double dt, double alpha)
{
	(void)dt;
}

Scene GetScenePlay()
{
	return (Scene){ init, update, fixed_update, draw, true };
}

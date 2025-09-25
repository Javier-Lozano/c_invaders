#include "common.h"

static void init(GameContext *game)
{
	(void)game;
}

static void update(GameContext *game)
{
	(void)game;
}

static void fixed_update(GameContext *game)
{
	(void)game;
}

static void draw(GameContext *game)
{
	(void)game;
}

Scene SetSceneTitle()
{
	return (Scene){ init, update, fixed_update, draw, true };
}

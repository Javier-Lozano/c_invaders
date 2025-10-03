#include "SDL.h"
#include "game.h"
#include "scene.h"
#include "transition.h"
#include "graphics.h"
#include "input.h"
#include "macros.h"

/////

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
	SDL_SetRenderDrawColor(game->renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(game->renderer, NULL);
}

Scene GetScenePlay()
{
	return (Scene){ init, update, fixed_update, draw, true };
}

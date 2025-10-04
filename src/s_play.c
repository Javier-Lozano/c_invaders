#include "SDL.h"
#include "game.h"
#include "scene.h"
#include "transition.h"
#include "graphics.h"
#include "input.h"
#include "macros.h"

typedef enum {
	STATE_INTRO,
	STATE_PLAY,
	STATE_PAUSE,
	STATE_CONFIG,
	STATE_GAMEOVER
} SceneState;

static SceneState g_State;
static float      g_Timer;

/////

static void init(GameContext *game)
{
	(void)game;
	g_State = STATE_INTRO;
	g_Timer = 0.0f;
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

Scene GetScenePlay()
{
	return (Scene){ init, update, fixed_update, draw, true };
}

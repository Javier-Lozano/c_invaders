#include "common.h"

typedef enum {
	STATE_INTRO,
	STATE_IDLE,
	STATE_CONFIG,
} StateID;

typedef enum {
	SELECT_PLAY = 1,
	SELECT_CONFIG,
	SELECT_EXIT
} SelectID;

static struct {
	StateID  state;
	SelectID select;
	float    offset[2];
} g_Scene;

const unsigned char g_TM_C[] = { // 8x8
	   0,   0,0xF3,0xF0,0xF0,0xF4,   0,   0,
	   0,0xF3,0xF2,0xF7,0xF7,0xF1,0xF4,   0,
	0xF3,0xF2,0xF3,0xF0,0xF0,0xF4,0xF1,0xF4,
	0xF0,0xF5,0xF0,0xFA,0xF9,0xFA,   0,0xF0,
	0xF0,0xF5,0xF0,0xFC,0xFF,0xFC,   0,0xF0,
	0xF1,0xF4,0xF1,0xF0,0xF0,0xF2,0xF3,0xF2,
	   0,0xF1,0xF4,0xF8,0xF8,0xF3,0xF2,   0,
	   0,   0,0xF1,0xF0,0xF0,0xF2,   0,   0,
};

const unsigned char g_TM_Invaders[] = { // 24x5
	0xF0,0xF0,0xF5,0xF6,0xF5,0xF6,0xF0,   0,0xF0,0xFF,0xF0,0xF4,0xF5,0xF0,0xF4,0xF5,0xF0,0xF0,0xF5,0xF0,0xF4,0xFF,0xF0,0xF4,
	0xF5,0xF6,0xF5,0xF0,0xF3,0xF6,0xF0,   0,0xF0,0xF5,0xF6,0xF0,0xF5,0xF6,0xF0,0xF5,0xF6,   0,0xF5,0xF6,0xF0,0xF5,0xF6,0xF9,
	0xF5,0xF6,0xF5,0xF0,0xF0,0xF6,0xF0,0xF7,0xF0,0xF5,0xF0,0xF0,0xF5,0xF6,0xF0,0xF5,0xF0,0xF6,0xF5,0xF0,0xF4,0xF9,0xF0,0xF4,
	0xF5,0xF6,0xF5,0xF6,0xF1,0xF6,0xF5,0xF0,0xF6,0xF5,0xF6,0xF0,0xF5,0xF6,0xF0,0xF5,0xF6,   0,0xF5,0xF6,0xF0,0xFF,   0,0xF0,
	0xF0,0xF0,0xF5,0xF6,0xF5,0xF6,   0,0xF0,   0,0xF5,0xF6,0xF0,0xF5,0xF0,0xF2,0xF5,0xF0,0xF0,0xF5,0xF6,0xF0,0xF9,0xF0,0xF2,
};

static void init(GameContext *game)
{
	(void)game;

	g_Scene.state  = STATE_INTRO;
	g_Scene.select = SELECT_PLAY;
	g_Scene.offset[0] = WINDOW_H;
	g_Scene.offset[1] = WINDOW_H;
}

static void update(GameContext *game)
{
	if (game->transition.state != TRANS_NONE)
	{
		if (game->transition.state == TRANS_WAIT && g_Scene.select == SELECT_PLAY)
			game->is_running = false;

		return;
	}

	switch(g_Scene.state)
	{
		case STATE_INTRO:
			if (g_Scene.offset[0] < 0 || IsAnyKeyPressed())
			{
				g_Scene.offset[1] = g_Scene.offset[0] = 0;
				g_Scene.state = STATE_IDLE;
			}
			break;
		case STATE_IDLE:
			{
				char up     = IsKeyPressed(SDLK_UP);
				char down   = IsKeyPressed(SDLK_DOWN);
				bool accept = IsKeyPressed(SDLK_RETURN);
				
				if (up || down)
				{
					g_Scene.select += (down - up);
					g_Scene.select = (CLAMP_WRAP(g_Scene.select, SELECT_PLAY, SELECT_EXIT));
				}

				if (g_Scene.select == SELECT_PLAY && accept)
				{
					StartTransition(game, 2, 1);
				}
				else if (g_Scene.select == SELECT_CONFIG && accept)
				{
				}
				else if (g_Scene.select == SELECT_EXIT && accept)
				{
					game->is_running = false;
				}
			}
			break;
		case STATE_CONFIG:
			// TODO: Config Screen
			break;
	}
}

static void fixed_update(GameContext *game)
{
	(void)game;

	const double fixed_dt = 1.0f / FPS;

	if (g_Scene.state == STATE_INTRO)
	{
		g_Scene.offset[1] = g_Scene.offset[0];
		g_Scene.offset[0] -= fixed_dt * 160;
	}
}

static void draw(GameContext *game)
{
	(void)game;

	const double fixed_dt = 1.0f / FPS;

	float offset = INTERPOLATE(g_Scene.offset[1], g_Scene.offset[0], game->accumulator / fixed_dt);

	// Draw Tilemaps
	for (int y = 0; y < 8; y++)
	{
		for(int x = 0; x < 8; x++)
		{
			int i  = (y * 8) + x;
			int xx = (x * 8) + 88;
			int yy = (y * 8) + 64 + offset;
			DrawTileRGBA(game->renderer, g_TM_C[i], xx, yy, (i < 32) ? 0x00FF00FF: 0x00FFFFFF);
		}
	}
	for (int y = 0; y < 5; y++)
	{
		for(int x = 0; x < 24; x++)
		{
			int i = (y * 24) + x;
			int xx = (x * 8) + 24;
			int yy = (y * 8) + 144 + offset;
			DrawTileRGBA(game->renderer, g_TM_Invaders[i], xx, yy, (i < 48) ? 0xFF00FFFF : 0xFFFF00FF);
		}
	}

	// Draw Interface
	if (g_Scene.state != STATE_INTRO)
	{
		DrawTextRGBA(game->renderer, "HI-Score", 88, 8, 0xFFFF00FF);
		DrawTextRGBA(game->renderer, "%d", 88, 24, 0xFFFFFFFF, game->highscore);

		DrawTextRGBA(game->renderer, "START", 104, 200, (g_Scene.select == SELECT_PLAY) ? 0xFFFF00FF : 0x808000FF);
		DrawTextRGBA(game->renderer, "CONFIGURE", 104, 216, (g_Scene.select == SELECT_CONFIG) ? 0xFFFF00FF : 0x808000FF);
		DrawTextRGBA(game->renderer, "EXIT", 104, 232, (g_Scene.select == SELECT_EXIT) ? 0xFFFF00FF : 0x808000FF);

		DrawTextRGBA(game->renderer, "CREDITS (2025)",  56, 256, 0xFFFF00FF);
		DrawTextRGBA(game->renderer, "PROGRAMMING",     24, 272, 0xFF0000FF);
		DrawTextRGBA(game->renderer, "HIGHWIND",       120, 272, 0x00FFFFFF);
		DrawTextRGBA(game->renderer, "SFX FROM",        48, 288, 0xFF0000FF);
		DrawTextRGBA(game->renderer, "SHIRU'S NESSFX", 120, 288, 0x00FFFFFF);
	}

	DrawTextRGBA(game->renderer, "%d", 0, 0, 0xFFFF00FF, g_Scene.select);
}

Scene SetSceneTitle()
{
	return (Scene) { init, update, fixed_update, draw, true };
}

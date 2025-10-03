#include "SDL.h"
#include "game.h"
#include "scene.h"
#include "transition.h"
#include "graphics.h"
#include "input.h"
#include "macros.h"

typedef enum {
	STATE_INTRO,
	STATE_IDLE,
	STATE_SCORE
} SceneState;

typedef enum {
	SELECT_PLAY = 1,
	SELECT_CONFIG,
	SELECT_EXIT
} SelectID;

static SceneState g_State;
static SelectID   g_Select;
static float      g_Offset[2];
static float      g_Timer;

/////

const unsigned char g_Tilemap_1[] = { // 8x8
	   0,   0,0xF3,0xF0,0xF0,0xF4,   0,   0,
	   0,0xF3,0xF2,0xF7,0xF7,0xF1,0xF4,   0,
	0xF3,0xF2,0xF3,0xF0,0xF0,0xF4,0xF1,0xF4,
	0xF0,0xF5,0xF0,0xFA,0xF9,0xFA,   0,0xF0,
	0xF0,0xF5,0xF0,0xFC,0xFB,0xFC,   0,0xF0,
	0xF1,0xF4,0xF1,0xF0,0xF0,0xF2,0xF3,0xF2,
	   0,0xF1,0xF4,0xF8,0xF8,0xF3,0xF2,   0,
	   0,   0,0xF1,0xF0,0xF0,0xF2,   0,   0,
};

const unsigned char g_Tilemap_2[] = { // 24x5
	0xF0,0xF0,0xF5,0xF6,0xF5,0xF6,0xF0,   0,0xF0,0xFB,0xF0,0xF4,0xF5,0xF0,0xF4,0xF5,0xF0,0xF0,0xF5,0xF0,0xF4,0xFB,0xF0,0xF4,
	0xF5,0xF6,0xF5,0xF0,0xF3,0xF6,0xF0,   0,0xF0,0xF5,0xF6,0xF0,0xF5,0xF6,0xF0,0xF5,0xF6,   0,0xF5,0xF6,0xF0,0xF5,0xF6,0xF9,
	0xF5,0xF6,0xF5,0xF0,0xF0,0xF6,0xF0,0xF7,0xF0,0xF5,0xF0,0xF0,0xF5,0xF6,0xF0,0xF5,0xF0,0xF6,0xF5,0xF0,0xF4,0xF9,0xF0,0xF4,
	0xF5,0xF6,0xF5,0xF6,0xF1,0xF6,0xF5,0xF0,0xF6,0xF5,0xF6,0xF0,0xF5,0xF6,0xF0,0xF5,0xF6,   0,0xF5,0xF6,0xF0,0xFF,   0,0xF0,
	0xF0,0xF0,0xF5,0xF6,0xF5,0xF6,   0,0xF0,   0,0xF5,0xF6,0xF0,0xF5,0xF0,0xF2,0xF5,0xF0,0xF0,0xF5,0xF6,0xF0,0xF9,0xF0,0xF2,
};

static void draw_tilemap(SDL_Renderer *renderer, float offset)
{
	for (int y = 0; y < 8; y++)
	{
		for(int x = 0; x < 8; x++)
		{
			int i  = (y * 8) + x;
			int xx = (x * 8) + 88;
			int yy = (y * 8) + 64 + offset;
			DrawTileRGBA(renderer, g_Tilemap_1[i], xx, yy, (i < 32) ? 0x00FF00FF: 0x00FFFFFF);
		}
	}
	for (int y = 0; y < 5; y++)
	{
		for(int x = 0; x < 24; x++)
		{
			int i = (y * 24) + x;
			int xx = (x * 8) + 24;
			int yy = (y * 8) + 144 + offset;
			DrawTileRGBA(renderer, g_Tilemap_2[i], xx, yy, (i < 48) ? 0xFF00FFFF : 0xFFFF00FF);
		}
	}
}

#define STARS (100)
#define DEPTH (500)
static float star_x[STARS];
static float star_y[STARS];
static float star_z[STARS];

static void draw_background(SDL_Renderer *renderer, double dt)
{
	const double speed = dt * INNER_FPS * 2;
	const double mid_z = DEPTH / 2;

	for(int i = 0; i < STARS; i++)
	{
		star_z[i] += speed;
		if (star_z[i] > DEPTH)
			star_z[i] -= mid_z;

		float z = star_z[i] / DEPTH;
		float x = (star_x[i] * z) + WINDOW_W / 2;
		float y = (star_y[i] * z) + WINDOW_H / 2;
		float a = (star_z[i] - mid_z) / mid_z;
		if (a > 0.5f)
			a = (1 - a);
		SDL_SetRenderDrawColor(renderer, 255 * (i & 1), 255 * (i & 2), 255 * (i & 4), 512 * a);
		SDL_RenderDrawPointF(renderer, x, y);
	}
}

/////

static void init(GameContext *game)
{
	(void)game;

	// Scene State
	g_State  = STATE_INTRO;
	g_Select = SELECT_PLAY;
	g_Offset[0] = g_Offset[1] = WINDOW_H;

	// Starts
	for(int i = 0; i < STARS; i++)
	{
		star_x[i] = (rand() % WINDOW_W) - (WINDOW_W / 2);
		star_y[i] = (rand() % WINDOW_H) - (WINDOW_H / 2);
		star_z[i] = (rand() % (DEPTH / 2)) + (DEPTH / 2);
	}
}

static void update(GameContext *game)
{
	// Switch to Play Scene
	TransState t_state = GetTransitionState();

	if (t_state != TRANS_NONE)
	{
		if (t_state == TRANS_WAIT && g_Select == SELECT_PLAY)
			SetScene(SCENE_PLAY);

		return;
	}

	switch(g_State)
	{
		case STATE_INTRO:
			if (g_Offset[0] < 0)
				g_State = STATE_IDLE;
			break;
		case STATE_IDLE:
			// Timer to Scoreboard
			g_Timer += game->elapsed_time;
			if (g_Timer > 10.0f)
			{
				g_State = STATE_SCORE;
				g_Timer = 0.0f;
			}

			// Input
			{
				char up     = IsKeyPressed(SDLK_UP);
				char down   = IsKeyPressed(SDLK_DOWN);
				bool accept = IsKeyPressed(SDLK_RETURN);

				if (up || down)
					g_Select = WRAP(g_Select + (down - up), SELECT_PLAY, SELECT_EXIT);

				if (g_Select == SELECT_PLAY && accept)
					StartTransition(1);
				else if (g_Select == SELECT_EXIT && accept)
					game->is_running = false;
				//else if (g_Select == SELECT_CONFIG && accept)
			}

			break;
		case STATE_SCORE:
			// Timer to Intro
			g_Timer += game->elapsed_time;
			if (g_Timer > 10.0f)
			{
				g_State = STATE_INTRO;
				g_Timer = 0.0f;
				g_Offset[0] = g_Offset[1] = WINDOW_H;
			}
			break;
	}

	// Reset Timer / Skip Intro
	if (IsAnyKeyPressed())
	{
		g_State = STATE_IDLE;
		g_Timer = 0.0f;
	}
}

static void fixed_update(GameContext *game)
{
	(void)game;

	const double fixed_dt = 1.0f / INNER_FPS;

	if (g_State == STATE_INTRO)
	{
		g_Offset[1] = g_Offset[0];
		g_Offset[0] -= fixed_dt * INNER_FPS * 3;
	}
}

static void draw(GameContext *game)
{
	const double fixed_dt = 1.0f / INNER_FPS;

	draw_background(game->renderer, game->elapsed_time);

	switch(g_State)
	{
		case STATE_INTRO:
			draw_tilemap(game->renderer, LERP(g_Offset[1], g_Offset[0], game->accumulator / fixed_dt));
			break;
		case STATE_IDLE:
			draw_tilemap(game->renderer, 0);
			DrawTextRGBA(game->renderer, "HI-Score", 88, 8, 0xFFFF00FF);
			DrawTextRGBA(game->renderer, "%d", 88, 24, 0xFFFFFFFF, game->settings.highscore[0]);

			DrawTextRGBA(game->renderer, "START",    104, 200, (g_Select == SELECT_PLAY)   ? 0xFFFF00FF : 0x808000FF);
			DrawTextRGBA(game->renderer, "SETTINGS", 104, 216, (g_Select == SELECT_CONFIG) ? 0xFFFF00FF : 0x808000FF);
			DrawTextRGBA(game->renderer, "EXIT",     104, 232, (g_Select == SELECT_EXIT)   ? 0xFFFF00FF : 0x808000FF);

			DrawTextRGBA(game->renderer, "CREDITS (2025)",  56, 256, 0xFFFF00FF);
			DrawTextRGBA(game->renderer, "PROGRAMMING",     24, 272, 0xFF0000FF);
			DrawTextRGBA(game->renderer, "HIGHWIND",       120, 272, 0x00FFFFFF);
			DrawTextRGBA(game->renderer, "SFX FROM",        48, 288, 0xFF0000FF);
			DrawTextRGBA(game->renderer, "SHIRU'S NESSFX", 120, 288, 0x00FFFFFF);
#if DEBUG
			DrawTextRGBA(game->renderer, "TIME: %f",   0, 0, 0xFFFF00A0, g_Timer);
			DrawTextRGBA(game->renderer, "SELECT: %d", 0, 8, 0xFFFF00A0, g_Select);
#endif
			break;
		case STATE_SCORE:
#if DEBUG
			DrawTextRGBA(game->renderer, "TIME: %f", 0, 0, 0xFF00FFA0, g_Timer);
#endif
			break;
	}
}

Scene GetSceneTitle()
{
	return (Scene){ init, update, fixed_update, draw, true };
}

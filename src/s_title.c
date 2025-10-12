#include "invaders.h"

///// Scene

typedef enum {
	STATE_INTRO,
	STATE_IDLE,
	STATE_CONFIG,
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

const unsigned int g_Colors[] = {
	0xFF0000FF, /* Red */
	0xFF8000FF, /* Orange*/
	0xFFFF00FF, /* Yellow */
	0x80FF00FF, /* Yellowish Green */
	0x00FF00FF, /* Green*/
	0x00FF80FF, /* Bluish Green*/
	0x00FFFFFF, /* Cyan */
	0x0080FFFF, /* Azure */
	0x0000FFFF, /* Blue */
	0x8000FFFF, /* Violet */
	0xFF00FFFF, /* Magenta */
	0xFF0080FF, /* Pink */
};

///// Title Tilemaps

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

static void draw_tilemap(SDL_Renderer *renderer, int offset)
{
	for (int i = 0; i < (64); i++)
	{
		unsigned int color = (i < 32) ? 0x00FF00FF : 0x00FFFFFF;
		int xx = (i % 8) * 8;
		int yy = (i / 8) * 8;
		DrawTileRGBA(renderer, g_Tilemap_1[i], xx + 88, yy + 66 + offset, color);
	}
	for (int i = 0; i < (120); i++)
	{
		unsigned int color = (i < 48) ? 0xFF00FFFF : 0xFFFF00FF;
		int xx = (i % 24) * 8;
		int yy = (i / 24) * 8;
		DrawTileRGBA(renderer, g_Tilemap_2[i], xx + 24, yy + 144 + offset, color);
	}
}

///// Scoreboard

static int g_CycleCount;
static int g_IndexShift;
static void draw_scoreboard(SDL_Renderer *renderer, Settings *settings)
{
	for (int y = 0; y < 10; y++)
	{
		int c = (y + g_IndexShift) % 12;
		for (int x = 0; x < 10; x++)
			DrawTextRGBA(renderer, "%s", 32, (y*16) + 80, g_Colors[c], settings->names[y]);

		DrawTextRGBA(renderer, "%d", 128, (y*16) + 80, g_Colors[c], settings->highscore[y]);
	}

	g_CycleCount++;
	if (g_CycleCount > 30)
	{
		g_CycleCount = 0;
		g_IndexShift = (g_IndexShift+1) % 12;
	}
}

///// Background

#define STARS (150)
#define DEPTH (100)
static float g_StarX[STARS];
static float g_StarY[STARS];
static float g_StarZ[STARS];

static void draw_background(SDL_Renderer *renderer, double dt)
{
	const double speed = dt * 60;
	const double mid_z = DEPTH / 2;

	for(int i = 0; i < STARS; i++)
	{
		g_StarZ[i] += speed;
		if (g_StarZ[i] > DEPTH)
			g_StarZ[i] -= mid_z;

		float z = g_StarZ[i] / DEPTH;
		float x = (g_StarX[i] * z) + WINDOW_W / 2;
		float y = (g_StarY[i] * z) + WINDOW_H / 2;

		unsigned char r = ((i) & 1) ? 255 : 0;
		unsigned char g = ((i) & 2) ? 255 : 0;
		unsigned char b = ((i) & 4) ? 255 : 0;
		float a = (g_StarZ[i] - mid_z) / mid_z;

		if (a > 0.5f)
			a = (1 - a);

		SDL_SetRenderDrawColor(renderer, r, g, b, 512 * a);
		SDL_RenderDrawPointF(renderer, x, y);
	}
}

///// 

static void init()
{
	// Scene State
	g_State  = STATE_INTRO;
	g_Select = SELECT_PLAY;
	g_Offset[0] = g_Offset[1] = WINDOW_H;

	// Stars
	for(int i = 0; i < STARS; i++)
	{
		g_StarX[i] = (rand() % WINDOW_W) - (WINDOW_W / 2);
		g_StarY[i] = (rand() % WINDOW_H) - (WINDOW_H / 2);
		g_StarZ[i] = (rand() % (DEPTH / 2)) + (DEPTH / 2);
	}
}

static void update(GameContext *game, double dt)
{
	// Switch to Play Scene
	TransState t_state = GetTransitionState();

	if (t_state != TRANS_NONE)
	{
		if (t_state == TRANS_WAIT && g_Select == SELECT_PLAY)
			SwitchScene(SCENE_PLAY);

		return;
	}

	// Input
	char input  = IsKeyPressed(SDLK_DOWN) - IsKeyPressed(SDLK_UP);
	bool accept = IsKeyPressed(SDLK_RETURN);
	bool any    = IsAnyKeyPressed();

	// Reset Timer / Skip Intro
	if (any)
		g_Timer = 0.0f;

	switch(g_State)
	{
		case STATE_INTRO:
			if (g_Offset[0] < 0 || any)
				g_State = STATE_IDLE;
			break;
		case STATE_IDLE:
			if (input)
				g_Select = WRAP(g_Select + input, SELECT_PLAY, SELECT_EXIT);

			if (accept)
			{
				if (g_Select == SELECT_PLAY)
					StartTransition(1);
				if (g_Select == SELECT_CONFIG)
					g_State = STATE_CONFIG;
				if (g_Select == SELECT_EXIT)
					game->is_running = false;
			}

			// Timer to Scoreboard
			g_Timer += dt;
			if (g_Timer > 10.0f)
			{
				g_State = STATE_SCORE;
				g_Timer = 0.0f;
			}

			// Reset Timer
			if (IsKeyPressed(SDLK_m))
				g_State = STATE_SCORE;

			break;
		case STATE_CONFIG:
			UpdateSettings(game->window, game->renderer, &game->settings);

			// Return to IDLE
			if (IsKeyPressed(SDLK_ESCAPE))
				g_State = STATE_IDLE;

			break;
		case STATE_SCORE:
			// Return to IDLE
			if (any)
				g_State = STATE_IDLE;

			// Timer to Intro
			g_Timer += dt;
			if (g_Timer > 10.0f)
			{
				g_State = STATE_INTRO;
				g_Timer = 0.0f;
				g_Offset[0] = g_Offset[1] = WINDOW_H;
			}
			break;
	}
}

static void fixed_update(GameContext *game, double fixed_dt)
{
	(void)game;

	if (g_State == STATE_INTRO)
	{
		g_Offset[1] = g_Offset[0];
		g_Offset[0] -= fixed_dt * WINDOW_H;
	}
}

static void draw(GameContext *game, double dt, double alpha)
{
	draw_background(game->renderer, dt);

	switch(g_State)
	{
		case STATE_INTRO:
			draw_tilemap(game->renderer, LERP(g_Offset[1], g_Offset[0], alpha));
			break;
		case STATE_IDLE:
			draw_tilemap(game->renderer, 0);
			DrawTextRGBA(game->renderer, "HI-Score", 88, 8, 0xFFFF00FF);
			DrawTextRGBA(game->renderer, "%d", 88, 24, 0xFFFFFFFF, game->settings.highscore[0]);

			DrawTextRGBA(game->renderer, "START",    104, 200, (g_Select == SELECT_PLAY)   ? 0xFFFF00FF : 0x808080FF);
			DrawTextRGBA(game->renderer, "SETTINGS", 104, 216, (g_Select == SELECT_CONFIG) ? 0xFFFF00FF : 0x808080FF);
			DrawTextRGBA(game->renderer, "EXIT",     104, 232, (g_Select == SELECT_EXIT)   ? 0xFFFF00FF : 0x808080FF);

			DrawTextRGBA(game->renderer, "CREDITS (2025)",  56, 256, 0xFFFF00FF);
			DrawTextRGBA(game->renderer, "PROGRAMMING",     24, 272, 0xFF0000FF);
			DrawTextRGBA(game->renderer, "HIGHWIND",       120, 272, 0x00FFFFFF);
			DrawTextRGBA(game->renderer, "SFX FROM",        48, 288, 0xFF0000FF);
			DrawTextRGBA(game->renderer, "SHIRU'S NESSFX", 120, 288, 0x00FFFFFF);
			break;
		case STATE_CONFIG:
			DrawSettings(game->renderer, &game->settings);
			DrawSettings(game->renderer, &game->settings);
			DrawText(game->renderer, "^7PRESS [^1ESC^7] TO RETURN", 32, 304);
			break;
		case STATE_SCORE:
			DrawTextRGBA(game->renderer, "HIGHSCORES", 80, 48, 0xFF0000FF);
			draw_scoreboard(game->renderer, &game->settings);
			break;
	}
}

Scene GetSceneTitle()
{
	return (Scene){ init, update, fixed_update, draw, true };
}

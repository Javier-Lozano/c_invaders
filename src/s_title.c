#include "invaders.h"
#include <time.h>

// Scene

typedef enum {
	STATE_INTRO,
	STATE_IDLE,
	STATE_CONFIG,
	STATE_SCORE
} StateID;

typedef enum {
	MENU_PLAY,
	MENU_CONFIG,
	MENU_EXIT
} MenuID;

static struct Scene {
	StateID state;
	MenuID  menu;
	float   clock;
} g_Scene;

// Stars
#define STAR_COUNT (200)
#define STAR_DEPTH (100)

static struct Stars {
	float x[STAR_COUNT];
	float y[STAR_COUNT];
	float z[STAR_COUNT];
	float alpha;
} g_Stars;

// Tilemap Control
enum {
	TM_WAIT    = 1,
	TM_1_INTRO = 2,
	TM_2_INTRO = 4
};

static struct TMControl {
	float         clock;
	int           ticks;
	int           effect[2];
	unsigned char flags;
} g_TMControl;

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

// Scoreboard
static struct Scoreboard {
	float timer;
	float clock;
	float hue;
} g_Scoreboard;

/////

static void stars_draw(SDL_Renderer *renderer, struct Stars *s, double dt)
{

	for (int i = 0; i < STAR_COUNT; i++)
	{
		s->z[i] += dt * 60;
		if (s->z[i] > STAR_DEPTH)
			s->z[i] -= STAR_DEPTH;

		float z = s->z[i] / STAR_DEPTH;
		float x = s->x[i] * (z + 0.3f);
		float y = s->y[i] * (z + 0.3f);

		unsigned char r, g, b;
		HSVToRGB(i * 10, 1.0f, 1.0f, &r, &g, &b);

		int alpha = Clamp(SDL_sin(3.14f * z) * 255 * s->alpha, 0, 255);

		SDL_SetRenderDrawColor(renderer, r, g, b, alpha);

		SDL_RenderDrawPointF(renderer, x + (WINDOW_W / 2), y + (WINDOW_H / 2));
	}

	if (g_Scene.state == STATE_IDLE && s->alpha < 1.0f)
		s->alpha += dt;
}

static void tilemap_draw(SDL_Renderer *renderer, const unsigned char *tilemap, int x, int y, int w, int h, int check, bool flag, unsigned int c1, unsigned int c2)
{
	const int half = (h / 2) * w;

	for (int i = 0; i < w * h; i++)
	{
		int tile = tilemap[i];

		if (flag)
		{
			if (i >= check)
				tile = 0xF0;
		}
		else
		{
			tile = 0xF0;
			if (i >= check)
				break;
		}
		
		int tx = (i % w) * 8;
		int ty = (i / w) * 8;
		DrawTileRGBA(renderer, tile, tx + x, ty + y, (i < half) ? c1 : c2);
	}
}

static void tilemap_anim_draw(SDL_Renderer *renderer, struct TMControl *tm, double dt)
{
	tilemap_draw(renderer, g_Tilemap_1, 88,  64,  8, 8, tm->effect[0] *  8, tm->flags & TM_1_INTRO, 0x00FF00FF, 0x00FFFFFF);
	tilemap_draw(renderer, g_Tilemap_2, 24, 144, 24, 5, tm->effect[1] * 24, tm->flags & TM_2_INTRO, 0xFF00FFFF, 0xFFFF00FF);

	if (tm->effect[0] == 8 && tm->effect[1] == 5)
		return;

	tm->clock += dt;

	// Wait Mode
	if (tm->flags & TM_WAIT)
	{
		if (tm->clock >= 0.5f)
		{
			tm->clock = 0.0f;
			tm->flags &= ~TM_WAIT;
		}

		return;
	}

	// Active Mode
	if (tm->clock <= 2.0f / 60.0f)
		return;

	tm->clock = 0;

	// "C" Tilemap Animation
	if (tm->effect[0] < 8)
	{
		tm->effect[0]++;

		if (tm->effect[0] == 8)
		{
			if (!(tm->flags & TM_1_INTRO))
			{
				tm->effect[0] = 0;
				tm->flags |= TM_1_INTRO;
			}

			tm->flags |= TM_WAIT;
		}

		return;
	}

	// "INVADERS" Tilemap Animation
	if (tm->effect[1] < 5)
	{
		tm->effect[1]++;

		if (tm->effect[1] == 5)
		{
			if (!(tm->flags & TM_2_INTRO))
			{
				tm->effect[1] = 0;
				tm->flags |= TM_2_INTRO;
			}

			tm->flags |= TM_WAIT;
		}
	}
}

static void draw_scoreboard(SDL_Renderer *renderer, struct Scoreboard *board, Settings *settings, double dt)
{
	const float step = 4.0f / 60.0f;
	char temp_line[128];

	board->clock += dt;
	if (board->clock > step)
	{
		board->clock -= step;
		board->hue   += 5.0f;
	}

	for (int i = 0; i < 10; i++)
	{
		snprintf(temp_line, 128, "%2d %s  %10d", i + 1, settings->names + (i*11), settings->highscore[i]);

		for (int c = 0; temp_line[c] && c < 128; c++)
			DrawCharRGBA(renderer, temp_line[c], (c*8) + 16, (i*16) + 80, HueToRGB(board->hue + (i*15) + (c*5)));
	}
}

/////

static void init()
{
	// Stars
	srand(time(NULL));
	for (int i = 0; i < STAR_COUNT; i++)
	{
		g_Stars.x[i] = (rand() % WINDOW_W) - (WINDOW_W / 2);
		g_Stars.y[i] = (rand() % WINDOW_H) - (WINDOW_H / 2);
		g_Stars.z[i] = rand() % STAR_DEPTH;
	}
}

static void update(GameContext *game, double dt)
{
	// Switch to Play Scene
	TransState t_state = GetTransitionState();

	if (t_state != TRANS_NONE)
	{
		if (t_state == TRANS_WAIT && g_Scene.menu == MENU_PLAY)
			SwitchScene(SCENE_PLAY);

		return;
	}

	// Input
	char input  = IsKeyPressed(SDLK_DOWN) - IsKeyPressed(SDLK_UP);
	bool accept = IsKeyPressed(SDLK_RETURN);
	bool any    = IsAnyKeyPressed();

	// Reset Timer / Skip Intro
	if (any)
	{
		g_Scoreboard.timer = 0.0f;
		g_TMControl.effect[0] = 8;
		g_TMControl.effect[1] = 5;
		g_TMControl.flags     = TM_1_INTRO | TM_2_INTRO;
	}

	// Scene State
	switch (g_Scene.state)
	{
		case STATE_INTRO:
			if (g_TMControl.effect[0] == 8 && g_TMControl.effect[1] == 5)
				g_Scene.state = STATE_IDLE;
			break;
		case STATE_IDLE:
			if (input != 0)
			{
				g_Scene.menu = Wrap(g_Scene.menu + input, MENU_PLAY, MENU_EXIT);
				PlaySoundVol(SND_CURSOR, 20);
			}

			if (accept)
			{
				if (g_Scene.menu == MENU_PLAY)
					StartTransition(1);
				if (g_Scene.menu == MENU_CONFIG)
					g_Scene.state = STATE_CONFIG;
				if (g_Scene.menu == MENU_EXIT)
					game->is_running = false;

				PlaySoundVol(SND_CONFIRM, 20);
			}

			// Timer to Scoreboard
			g_Scoreboard.timer += dt;
			if (g_Scoreboard.timer > 10.0f)
			{
				g_Scoreboard.timer = 0.0f;
				g_Scene.state = STATE_SCORE;
			}

			// Switch to Scoreboard
			if (IsKeyPressed(SDLK_m))
				g_Scene.state = STATE_SCORE;
			break;
		case STATE_CONFIG:
			UpdateSettings(game->window, game->renderer, &game->settings);

			// Return to IDLE
			if (IsKeyPressed(SDLK_ESCAPE))
			{
				g_Scene.state = STATE_IDLE;
				PlaySoundVol(SND_CANCEL, 40);
			}
			break;
		case STATE_SCORE:
			// Return to IDLE
			if (any)
				g_Scene.state = STATE_IDLE;

			// Timer to Intro
			g_Scoreboard.timer += dt;
			if (g_Scoreboard.timer > 10.0f)
			{
				g_Scene.state = STATE_INTRO;
				g_Scoreboard.timer = 0.0f;
			}
			break;
	}
}

static void fixed_update(GameContext *game, double fixed_dt) {}

static void draw(GameContext *game, double dt, double alpha)
{
	stars_draw(game->renderer, &g_Stars, dt);

	switch (g_Scene.state)
	{
		case STATE_INTRO:
			tilemap_anim_draw(game->renderer, &g_TMControl, dt);
			break;
		case STATE_IDLE:
			tilemap_anim_draw(game->renderer, &g_TMControl, dt);

			DrawTextRGBA(game->renderer, "HI-Score", 88, 8, 0xFFFF00FF);
			DrawTextRGBA(game->renderer, "%d", 88, 24, 0xFFFFFFFF, game->settings.highscore[0]);

			DrawTextRGBA(game->renderer, "START",    104, 200, (g_Scene.menu == MENU_PLAY)   ? 0xFFFF00FF : 0x808080FF);
			DrawTextRGBA(game->renderer, "SETTINGS", 104, 216, (g_Scene.menu == MENU_CONFIG) ? 0xFFFF00FF : 0x808080FF);
			DrawTextRGBA(game->renderer, "EXIT",     104, 232, (g_Scene.menu == MENU_EXIT)   ? 0xFFFF00FF : 0x808080FF);

			DrawTextRGBA(game->renderer, "CREDITS (2025)",  56, 256, 0xFFFF00FF);
			DrawTextRGBA(game->renderer, "PROGRAMMING",     24, 272, 0xFF0000FF);
			DrawTextRGBA(game->renderer, "HIGHWIND",       120, 272, 0x00FFFFFF);
			DrawTextRGBA(game->renderer, "SFX FROM",        48, 288, 0xFF0000FF);
			DrawTextRGBA(game->renderer, "SHIRU'S NESSFX", 120, 288, 0x00FFFFFF);
			break;
		case STATE_CONFIG:
			DrawSettings(game->renderer, &game->settings);
			DrawText(game->renderer, "^7PRESS [^1ESC^7] TO RETURN", 32, 304);
			break;
		case STATE_SCORE:
			DrawTextRGBA(game->renderer, "HIGHSCORES", 80, 48, 0xFF0000FF);
			draw_scoreboard(game->renderer, &g_Scoreboard, &game->settings, dt);
			break;
	}
}

SceneTable GetSceneTitle()
{
	return (SceneTable){ init, update, fixed_update, draw, true };
}

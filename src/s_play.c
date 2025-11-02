#include "invaders.h"

typedef SDL_Rect  Rect;
typedef SDL_FRect FRect;

///// Scene

typedef enum {
	STATE_INTRO,
	STATE_PLAY,
	STATE_GAMEOVER
} SceneState;

/*
	TODO: Replace all global variables with global structs.
*/
static SceneState g_State;
static int        g_Level;
static int        g_Score;
static int        g_IntroRow;

///// Entity

#define ENT_COUNT (1+55+1+56)

static Rect g_Colliders[ENT_COUNT];
static bool g_IsAlive[ENT_COUNT];

static float g_PlayerSpd;
static float g_PlayerX[2];

static float g_PlayerBulletSpd;
static float g_PlayerBulletY[2];

static int g_EnemyIndex;
static int g_EnemyStep;    // 0000 0111 (Mask: 0x07)
static int g_EnemyDrop;    // 0011 0000 (Mask: 0x30)
static int g_EnemyDir;     // 0100 0000 (Mask: 0x40)
static bool g_EnemyChange; // 1000 0000 (Mask: 0x80)
//static Uint32 g_EnemyFlags;


///// UI

static void draw_ui(SDL_Renderer *renderer)
{
	// Text
	DrawTextRGBA(renderer, "PLAYER", 32, 16, 0xFFFFFFFF);
	DrawTextRGBA(renderer, "%d",     32, 32, 0xA0A0A0FF, g_Score);

	// Lines
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, 0,  48, WINDOW_W,  48);
	SDL_RenderDrawLine(renderer, 0, 288, WINDOW_W, 288);
}

static void init()
{
	Rect *player  = g_Colliders;
	Rect *aliens  = g_Colliders + 1;
	Rect *ufo     = g_Colliders + 56;
	Rect *bullets = g_Colliders + 57;

	g_Level = 1;

	// Player
	player->x = 114;
	player->y = 272;
	player->w = 12;
	player->h = 8;

	g_PlayerX[0] = g_PlayerX[1] = player->x;

	g_IsAlive[0] = true;

	// Aliens
	g_EnemyDir = 1;

	for (int i = 0; i < 55; i++)
	{
		g_IsAlive[i+1] = true;
		aliens[i].x =  ((i % 11) * 16) + 32;
		aliens[i].y = -((i / 11) * 16) - 8;
		aliens[i].w = 8;
		aliens[i].h = 8;

		if (i < 22)
		{
			aliens[i].x += 2;
			aliens[i].w += 4;
		}
		else if (i < 44)
		{
			aliens[i].x += 3;
			aliens[i].w += 2;
		}
		else if (i < 55)
		{
			aliens[i].x += 4;
		}
	}

	// UFO
	ufo->x = 224;
	ufo->w = 14;
	ufo->h = 8;

	// Bullets
	for (int i = 0; i < 56; i++)
	{
		bullets[i].w = 2;
		bullets[i].h = 10;
	}
}

static void update(GameContext *game, double dt)
{
	(void)game;
	(void)dt;
	Rect *player  = g_Colliders;
	Rect *bullets = g_Colliders + 57;
	bool *bullets_active = g_IsAlive + 57;

	switch (g_State)
	{
		case STATE_INTRO:
			break;
		case STATE_PLAY:
			// Player

			g_PlayerSpd = ((float)IsKeyDown(SDLK_RIGHT) - (float)IsKeyDown(SDLK_LEFT)) * 2.0f;

			// Player Bullet
			if (IsKeyPressed(SDLK_SPACE) && !g_IsAlive[57])
			{
				PlaySoundVol(SND_FIRE_PLAYER, 10);
				g_IsAlive[57] = true;
				bullets[0].x = player->x;
				bullets[0].y = player->y -8;
				g_PlayerBulletY[0] = g_PlayerBulletY[1] = bullets[0].y;
				g_PlayerBulletSpd = -4;
			}


			break;
		case STATE_GAMEOVER:
			break;
	}
}

static void fixed_update(GameContext *game, double fixed_dt)
{
	(void)game;
	(void)fixed_dt;

	Rect *aliens = g_Colliders + 1;
	Rect *player = g_Colliders;
	Rect *bullets = g_Colliders + 57;

	switch (g_State)
	{
		case STATE_INTRO:
			{
				int offset = g_IntroRow * 11;
				int limit  = CLAMP((g_Level * 16) + 64, 64, 192);

				PlaySoundVol(SND_STEP1 + g_EnemyStep, 20);

				for(int i = 0; i < 11; i++)
					aliens[i + offset].y += 8;

				g_IntroRow  = WRAP(g_IntroRow  + 1, 0, 4);
				g_EnemyStep = WRAP(g_EnemyStep + 1, 0, 3);

				if (aliens[54].y >=  limit)
					g_State = STATE_PLAY;
			}
			break;
		case STATE_PLAY:
			{
				// Player
				g_PlayerX[1] = g_PlayerX[0];
				g_PlayerX[0] += g_PlayerSpd;

				g_PlayerX[0] = CLAMP(g_PlayerX[0], 0, WINDOW_W - player->w);
				player->x = g_PlayerX[0];

				// Player Bullet
				if (g_IsAlive[57])
				{
					g_PlayerBulletY[1] = g_PlayerBulletY[0];
					g_PlayerBulletY[0] += g_PlayerBulletSpd;
					bullets[0].y = g_PlayerBulletY[0];

					if (bullets[0].y < 0)
					{
						PlaySoundVol(SND_HIT_MISS, 10);
						g_IsAlive[57] = false;
					}
				}

				// Aliens
				while (!g_IsAlive[g_EnemyIndex+1])
				{
					g_EnemyIndex++;

					if (g_EnemyIndex >= 55)
						g_EnemyIndex = 0;
				}

				if (g_EnemyIndex == 0)
				{
					PlaySoundVol(SND_STEP1 + g_EnemyStep, 20);
					g_EnemyStep = WRAP(g_EnemyStep + 1, 0, 3);
					if (g_EnemyChange)
					{
						g_EnemyDir *= -1;
						g_EnemyChange = false;
					}
					if (g_EnemyDrop == 1)
					{
						g_EnemyDrop = 0;
					}
					if (g_EnemyDrop == 2)
					{
						g_EnemyDrop = 1;
					}
				}

				if (g_EnemyDir == 1 && aliens[g_EnemyIndex].x + aliens[g_EnemyIndex].w + 2 >= WINDOW_W)
				{
					g_EnemyChange = true;
					g_EnemyDrop   = 2; 
				}
				if (g_EnemyDir == -1 && aliens[g_EnemyIndex].x - 2 <= 0)
				{
					g_EnemyChange = true;
					g_EnemyDrop   = 2; 
				}

				aliens[g_EnemyIndex].x += 2 * g_EnemyDir;
				if (g_EnemyDrop == 1)
					aliens[g_EnemyIndex].y += 8;

				g_EnemyIndex++;
				if (g_EnemyIndex >= 55)
					g_EnemyIndex = 0;
			}
			break;
		case STATE_GAMEOVER:
			break;
	}
}

static void draw(GameContext *game, double dt, double alpha)
{
	(void)dt;

	Rect *bullets = g_Colliders + 57;

	float x = LERP(g_PlayerX[1], g_PlayerX[0], alpha);
	FRect prect = {
		.x = x,
		.y = g_Colliders->y,
		.w = g_Colliders->w,
		.h = g_Colliders->h
	};
	SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
	SDL_RenderDrawRectF(game->renderer, &prect);

	if (g_IsAlive[57])
	{
		float y = LERP(g_PlayerBulletY[1], g_PlayerBulletY[0], alpha);
		FRect brect = {
			.x = bullets[0].x,
			.y = y,
			.w = bullets[0].w,
			.h = bullets[0].h
		};
		SDL_SetRenderDrawColor(game->renderer, 0, 255, 0, 255);
		SDL_RenderDrawRectF(game->renderer, &brect);
	}

	for(int i = 1; i < 56; i++)
	{
		if (g_IsAlive[i])
			SDL_SetRenderDrawColor(game->renderer, 255, 0, 0, 255);
		else
			SDL_SetRenderDrawColor(game->renderer, 255, 0, 255, 255);
		SDL_RenderDrawRect(game->renderer, &g_Colliders[i]);
	}

	draw_ui(game->renderer);
}

Scene GetScenePlay()
{
	return (Scene){ init, update, fixed_update, draw, true };
}

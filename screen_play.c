#include "common.h"
#include "events.h"
#include "input.h"
#include "assets.h"

#include "player.h"
#include "bullet.h"

enum {
	STATE_ROUND,
	STATE_INTRO,
	STATE_PLAY,
	STATE_PAUSE,
	STATE_GAMEOVER
};

typedef struct play_screen_st {
	u8     state;
	u8     round;
	u32    score;
	Player player;
//	Enemy  enemies[55];
	Bullet bullets[BULLET_COUNT];
} PlayScreen;

static bool do_once = true;

static void init(PlayScreen *screen)
{
	screen->state = STATE_PLAY;
	
	InitPlayer(&screen->player, false);
	screen->player.bullet = &screen->bullets[55];
}

static void update(GameContext *game, PlayScreen *screen, f64 dt)
{
	// Update Player
	UpdatePlayer(game, &screen->player, dt);
	UpdateBullet(screen->bullets, dt);
}

static void draw(SDL_Renderer *renderer, GameContext *game, PlayScreen *screen)
{
	// Draw Player
	DrawBullet(renderer, screen->bullets);
	DrawPlayer(renderer, game, &screen->player);

	// Draw UI
	SDL_RenderDrawLine(renderer, 0, 287, WINDOW_W, 287);
	DrawText(renderer, "LIVES", 16, 288, 0x00FF00FF);
	DrawText(renderer, "%d",    24, 296, 0x00FFFFFF, screen->player.lives);

	DrawText(renderer, "PLAYER", 16, 8, 0x00FFFFFF);
	DrawText(renderer, "%d", 16, 24, 0xFFFFFFFF, screen->score);
	DrawText(renderer, "HI-Score", 88, 8, 0xFFFF00FF);
	DrawText(renderer, "%d", 88, 24, 0xFFFF00FF, screen->score < game->score[0] ? game->score[0] : screen->score);
}

void ScreenPlay(GameContext *game)
{
	const f64 fixed_delta = 1.0f / game->fixed_framerate;
	static PlayScreen play_screen;
	PlayScreen dummy_screen;

	if (do_once)
	{
		init(&play_screen);
		do_once = false;
	}

	for(; game->accumulator > fixed_delta; game->accumulator -= fixed_delta)
	{
		ProcessInput();
		update(game, &play_screen, fixed_delta);
		ClearInput();
	}

	dummy_screen = play_screen;
	update(game, &dummy_screen, game->accumulator);
	draw(game->renderer, game, &dummy_screen);
}

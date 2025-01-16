#include "common.h"
#include "events.h"
#include "input.h"
#include "assets.h"

typedef struct title_screen_t{
	u8  state;
	i8  sel;
	f32 y_offset;
} TitleScreen;

static bool do_once = true;

const u8 g_TilemapC[] = { // 8x8
	   0,   0,0x73,0x70,0x70,0x74,   0,   0,
	   0,0x73,0x72,0x77,0x77,0x71,0x74,   0,
	0x73,0x72,0x73,0x70,0x70,0x74,0x71,0x74,
	0x70,0x75,0x70,0x7A,0x79,0x7A,   0,0x70,
	0x70,0x75,0x70,0x7C,0x7B,0x7C,   0,0x70,
	0x71,0x74,0x71,0x70,0x70,0x72,0x73,0x72,
	   0,0x71,0x74,0x78,0x78,0x73,0x72,   0,
	   0,   0,0x71,0x70,0x70,0x72,   0,   0,
};

const u8 g_TilemapInvaders[] = { // 24x5
	0x70,0x70,0x75,0x76,0x75,0x76,0x70,   0,0x70,0x7B,0x70,0x74,0x75,0x70,0x74,0x75,0x70,0x70,0x75,0x70,0x74,0x7B,0x70,0x74,
	0x75,0x76,0x75,0x70,0x73,0x76,0x70,   0,0x70,0x75,0x76,0x70,0x75,0x76,0x70,0x75,0x76,   0,0x75,0x76,0x70,0x75,0x76,0x79,
	0x75,0x76,0x75,0x70,0x70,0x76,0x70,0x77,0x70,0x75,0x70,0x70,0x75,0x76,0x70,0x75,0x70,0x76,0x75,0x70,0x74,0x79,0x70,0x74,
	0x75,0x76,0x75,0x76,0x71,0x76,0x75,0x70,0x76,0x75,0x76,0x70,0x75,0x76,0x70,0x75,0x76,   0,0x75,0x76,0x70,0x7B,   0,0x70,
	0x70,0x70,0x75,0x76,0x75,0x76,   0,0x70,   0,0x75,0x76,0x70,0x75,0x70,0x72,0x75,0x70,0x70,0x75,0x76,0x70,0x79,0x70,0x72,
};

static void init(TitleScreen *screen)
{
	screen->state = 0;
	screen->sel   = 0;
	screen->y_offset = WINDOW_H;
}

static void update(GameContext *game, TitleScreen *screen, f64 dt)
{
	if (screen->state == 0)
	{
		screen->y_offset -= dt * 180;
		if (screen->y_offset < 0 || IsAnyPressed())
		{
			screen->y_offset = 0;
			screen->state = 1;
		}
	}
	else if (screen->state == 1)
	{
		i8 up   = IsKeyPressed(SDLK_UP);
		i8 down = IsKeyPressed(SDLK_DOWN);
		i8 ret  = IsKeyPressed(SDLK_RETURN);

		// Selection
		if (up || down)
		{
			screen->sel += ((i8)down - (i8)up);
			screen->sel = CLAMP_WRAP(screen->sel, 0, 2);
			PlaySound(SFX_CURSOR);
		}

		// PLAY
		if (screen->sel == 0 && ret)
		{
			game->current_screen = ScreenPlay;
			PlaySound(SFX_SELECT);
		}

		// CONFIGURE
		if (screen->sel == 1 && ret)
		{
			//ctx->screen = SCREEN_CONFIG;
			screen->state = 2;
			PlaySound(SFX_SELECT);
		}

		// Exit
		if ((screen->sel == 2 && ret) || IsKeyPressed(SDLK_ESCAPE))
			game->is_running = 0;
	}
	else if (screen->state == 2)
	{
		if (IsKeyPressed(SDLK_ESCAPE))
		{
			screen->state = 1;
		}

		//UpdateConfigSubscreen();
	}

}

static void draw(SDL_Renderer *renderer, GameContext *game, TitleScreen *screen)
{
	// Draw Tilemap Letters
	for (int y = 0; y < 8; y++)
	{
		for(int x = 0; x < 8; x++)
		{
			int i  = (y * 8) + x;
			int xx = (x * SPRITE_SIZE) + 88;
			int yy = (y * SPRITE_SIZE) + 64 + screen->y_offset;
			DrawSpriteColor(renderer, g_TilemapC[i], xx, yy, (i < 32) ? 0x00FF00FF: 0x00FFFFFF);
		}
	}
	for (int y = 0; y < 5; y++)
	{
		for(int x = 0; x < 24; x++)
		{
			int i = (y * 24) + x;
			int xx = (x * SPRITE_SIZE) + 24;
			int yy = (y * SPRITE_SIZE) + 144 + screen->y_offset;
			DrawSpriteColor(renderer, g_TilemapInvaders[i], xx, yy, (i < 48) ? 0xFF00FFFF : 0xFFFF00FF);
		}
	}

	if (screen->state > 0)
	{
		DrawText(renderer, "HI-Score", 88, 8, 0xFFFF00FF);
		DrawText(renderer, "%d", 88, 24, 0xFFFF00FF, game->score[0]);

		DrawText(renderer, "START",     104, 200, (screen->sel == 0) ? 0xFFFF00FF : 0x808000FF);
		DrawText(renderer, "CONFIGURE", 104, 216, (screen->sel == 1) ? 0xFFFF00FF : 0x808000FF);
		DrawText(renderer, "EXIT",      104, 232, (screen->sel == 2) ? 0xFFFF00FF : 0x808000FF);

		DrawText(renderer, "CREDITS (2025)",  56, 256, 0xFFFF00FF);
		DrawText(renderer, "PROGRAMMING",     24, 272, 0xFF0000FF);
		DrawText(renderer, "HIGHWIND",       120, 272, 0x00FFFFFF);
		DrawText(renderer, "SFX FROM",        48, 288, 0xFF0000FF);
		DrawText(renderer, "SHIRU'S NESSFX", 120, 288, 0x00FFFFFF);
	}

	if (screen->state == 2)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xD0);
		SDL_RenderFillRect(renderer, NULL);
		//DrawConfigSubscreen();
	}
}

void ScreenTitle(GameContext *game)
{
	const f64 fixed_delta = 1.0f / game->fixed_framerate;
	static TitleScreen title_screen;
	TitleScreen dummy_screen;

	if (do_once)
	{
		init(&title_screen);
		do_once = false;
	}

	for(; game->accumulator > fixed_delta; game->accumulator -= fixed_delta)
	{
		ProcessInput();
		update(game, &title_screen, fixed_delta);
		ClearInput();
	}

	dummy_screen = title_screen;
	update(game, &dummy_screen, game->accumulator);
	draw(game->renderer, game, &dummy_screen);
}


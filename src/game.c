#include "common.h"

static void events(GameContext *game);
static void draw_screen(GameContext *game);
static void mod_screen(GameContext *game, char s_value, char a_value);

void GameUpdate(GameContext *game)
{
	const double fixed_dt = 1.0f / INNER_FPS;

	SDL_SetRenderDrawColor(game->renderer, 0x10, 0x10, 0x10, 0xFF);
	SDL_RenderClear(game->renderer);

	SDL_SetRenderTarget(game->renderer, game->fbuffer);
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0xFF);
	SDL_RenderClear(game->renderer);

	SetGraphicsColor(255, 255, 255, 255);

	events(game);

	InputStateUpdate();

	if (game->scene.is_starting)
	{
		game->scene.init(game);
		game->scene.is_starting = false;
	}

	game->scene.update(game);

	for(; game->accumulator > fixed_dt; game->accumulator -= fixed_dt)
	{
		game->scene.fixed_update(game);
		InputStateClear();
	}

	game->scene.draw(game);

	draw_screen(game);
}

static const Settings g_DefaultSettings = {
	.highscore = {57300, 37500, 30000, 25000, 20000, 19500, 19000, 15000, 10000, 5000},
	.names = {
		"BLOODY SABBATH  ",
		"HOLY DIVER      ",
		"FROZEN RAINBOW  ",
		"VET OF PSY WARS ",
		"SOLITUDE        ",
		"WICKED WAYS     ",
		"TOWER OF ABYSS  ",
		"JUSTICE FOR ALL ",
		"BLACK METAL     ",
		"DE MYSTERIIS DOM",
	},
	.scale = 2,
	.angle = 0,
	.vsync = false,
	.fullscreen = false
};

void LoadSaveFile(GameContext *game)
{
	bool error_read = 0;
	bool error_eof  = 0; 

	FILE *file = fopen("SAVE.DAT", "rb");
	if (file == NULL)
		goto DEFAULT_FILE;

	fread(&game->settings, sizeof(Settings), 1, file);
	error_read = ferror(file);
	error_eof  = feof(file);
	fclose(file);

	if (error_read || error_eof)
		goto DEFAULT_FILE;

#ifdef DEBUG
	printf("'SAVE.DAT' loaded.\n");
	printf("Scale:   %d\n", game->settings.scale);
	printf("Angle:   %d\n", game->settings.angle);
	printf("Vsync:   %d\n", game->settings.vsync);
	printf("Fullscr: %d\n", game->settings.fullscreen);
#endif

	return;

DEFAULT_FILE:
	printf("Creating 'SAVE.DAT'\n");
	game->settings = g_DefaultSettings;
	WriteSaveFile(game);
	return;
}

void WriteSaveFile(GameContext *game)
{
	FILE *file = fopen("SAVE.DAT", "wb");
	if (file == NULL)
	{
		printf("Error: Couldn't write to 'SAVE.DAT'\n");
		return;
	}

	fwrite(&game->settings, sizeof(Settings), 1, file);
	fclose(file);

#ifdef DEBUG
	printf("'SAVE.DAT' saved.\n");
	printf("Scale:   %d\n", game->settings.scale);
	printf("Angle:   %d\n", game->settings.angle);
	printf("Vsync:   %d\n", game->settings.vsync);
	printf("Fullscr: %d\n", game->settings.fullscreen);
#endif

	return;
}

//

void events(GameContext *game)
{
	const Uint8 *state = SDL_GetKeyboardState(NULL);
	bool shift = state[SDL_SCANCODE_RSHIFT] || state[SDL_SCANCODE_LSHIFT];
	bool alt   = state[SDL_SCANCODE_RALT]   || state[SDL_SCANCODE_LALT];

	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				game->is_running = false;
				break;
			case SDL_KEYDOWN:
				switch(event.key.keysym.sym)
				{
					case SDLK_RETURN:
						if (alt)
						{
							game->settings.fullscreen = !game->settings.fullscreen;
							SDL_SetWindowFullscreen(game->window, game->settings.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
							mod_screen(game, 0, 0);
						}
						break;
					case SDLK_PAGEUP:
						if (shift)
							mod_screen(game, 0, 1);
						else
							mod_screen(game, 1, 0);
						break;
					case SDLK_PAGEDOWN:
						if (shift)
							mod_screen(game, 0, -1);
						else
							mod_screen(game, -1, 0);
						break;
				}
				break;
		}
	}
}

void draw_screen(GameContext *game)
{
	int w, h;
	SDL_GetWindowSize(game->window, &w, &h);

	bool is_vertical = (game->settings.angle % 2) == 0;
	double angle = game->settings.angle * 90.0f;

	SDL_Rect rect = {
		.w = WINDOW_W * game->settings.scale,
		.h = WINDOW_H * game->settings.scale,
	};

	if (is_vertical)
	{
		rect.x = (w / 2) - (rect.w / 2);
		rect.y = (h / 2) - (rect.h / 2);
	}
	else
	{
		rect.x = (w / 2) - (rect.h / 2) + (40 * game->settings.scale);
		rect.y = (h / 2) - (rect.w / 2) - (40 * game->settings.scale);
	}

	SDL_SetRenderTarget(game->renderer, NULL);
	SDL_RenderCopyEx(game->renderer, game->fbuffer, NULL, &rect, angle, NULL, 0);
	SDL_RenderPresent(game->renderer);
}

void mod_screen(GameContext *game, char s_value, char a_value)
{
	SDL_DisplayMode display;
	int display_index = SDL_GetWindowDisplayIndex(game->window);
	if (SDL_GetCurrentDisplayMode(display_index, &display) != 0)
	{
		printf("%s\n", SDL_GetError());
		return;
	}

	// Rotate Screen
	if (a_value != 0)
		game->settings.angle = WRAP(game->settings.angle + a_value, 0, 3);

	bool is_vertical = (game->settings.angle % 2) == 0;
	int w = is_vertical ? WINDOW_W : WINDOW_H;
	int h = is_vertical ? WINDOW_H : WINDOW_W;

	// Scale Screen
	if (s_value != 0 || a_value != 0)
		game->settings.scale = CLAMP(game->settings.scale + s_value, 1, MIN(display.h / h, display.w / w));

	if (!game->settings.fullscreen)
	{
		SDL_SetWindowMinimumSize(game->window,
				w * game->settings.scale,
				h * game->settings.scale);

		SDL_SetWindowSize(game->window,
				w * game->settings.scale,
				h * game->settings.scale);

		SDL_SetWindowPosition(game->window,
				SDL_WINDOWPOS_CENTERED,
				SDL_WINDOWPOS_CENTERED);
	}
}


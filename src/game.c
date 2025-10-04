#include "SDL.h"
#include "game.h"
#include "scene.h"
#include "graphics.h"
#include "input.h"
#include "transition.h"
#include "macros.h"

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
							ModScreen(game->window, &game->settings, 0, 0);
						}
						break;
					case SDLK_PAGEUP:
						if (shift)
							ModScreen(game->window, &game->settings, 0, 1);
						else
							ModScreen(game->window, &game->settings, 1, 0);
						break;
					case SDLK_PAGEDOWN:
						if (shift)
							ModScreen(game->window, &game->settings, 0, -1);
						else
							ModScreen(game->window, &game->settings, -1, 0);
						break;
				}
				break;
		}
	}

	InputStateUpdate();
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

	UpdateTransition(game->renderer, game->elapsed_time);

	SDL_SetRenderTarget(game->renderer, NULL);
	SDL_RenderCopyEx(game->renderer, game->fbuffer, NULL, &rect, angle, NULL, 0);

	SDL_RenderPresent(game->renderer);
}

//

void GameLoop(GameContext *game)
{
	SDL_SetRenderDrawColor(game->renderer, 16, 16, 16, 255);
	SDL_RenderClear(game->renderer);

	SDL_SetRenderTarget(game->renderer, game->fbuffer);
	SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 255);
	SDL_RenderClear(game->renderer);

	SetGraphicsColor(255, 255, 255, 255);

	events(game);

	UpdateScene(game);

	draw_screen(game);
}


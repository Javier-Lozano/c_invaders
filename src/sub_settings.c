#include "SDL.h"
#include "game.h"
#include "scene.h"
#include "graphics.h"
#include "input.h"
#include "macros.h"

static char g_Select = 1;

void UpdateSettings(GameContext *game)
{
	char input  = IsKeyPressed(SDLK_DOWN) - IsKeyPressed(SDLK_UP);
	char left   = IsKeyPressed(SDLK_LEFT);
	char right  = IsKeyPressed(SDLK_RIGHT);
	bool accept = IsKeyPressed(SDLK_RETURN);
	g_Select = WRAP(g_Select + input, 1, 7);

	switch(g_Select)
	{
		case 1:
			if (left || right || accept)
			{
				game->settings.fullscreen = !game->settings.fullscreen;
				SDL_SetWindowFullscreen(game->window, game->settings.fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
				ModScreen(game->window, &game->settings, 0, 0);
			}
			break;
		case 2:
			if (left || right)
				ModScreen(game->window, &game->settings, right-left, 0);
			break;
		case 3:
			if (left || right)
				ModScreen(game->window, &game->settings, 0, right-left);
			break;
		case 4:
			// TODO: Volume Control
			break;
	}
}

void DrawSettings(SDL_Renderer *renderer, Settings *settings)
{
	DrawText(renderer, "^2C",       112, 16);
	DrawText(renderer, "^5INVADERS", 88, 32);
	DrawText(renderer, "^3SETTINGS", 88, 48);

	DrawTextRGBA(renderer, "FULLSCREEN         %s",     16,  64, g_Select == 1 ? 0xFF0000FF : 0x808080FF, settings->fullscreen ? "YES" : "NO");
	DrawTextRGBA(renderer, "SCALING            %d",     16,  80, g_Select == 2 ? 0xFF0000FF : 0x808080FF, settings->scale);
	DrawTextRGBA(renderer, "ORIENTATION        %d DEG", 16,  96, g_Select == 3 ? 0xFF0000FF : 0x808080FF, settings->angle * 90);
	DrawTextRGBA(renderer, "VOLUME             %d",     16, 112, g_Select == 4 ? 0xFF0000FF : 0x808080FF, 0);
	DrawTextRGBA(renderer, "RESET SETTINGS"      ,      16, 208, g_Select == 5 ? 0xFF0000FF : 0x808080FF);
	DrawTextRGBA(renderer, "RESET HIGH-SCORES"   ,      16, 224, g_Select == 6 ? 0xFF0000FF : 0x808080FF);
	DrawTextRGBA(renderer, "RESET ALL"           ,      16, 240, g_Select == 7 ? 0xFF0000FF : 0x808080FF);
}


#include "invaders.h"

static char g_SettingsSelect;

void UpdateSettings(SDL_Window *window, SDL_Renderer *renderer, Settings *settings)
{
	char input  = IsKeyPressed(SDLK_DOWN) - IsKeyPressed(SDLK_UP);
	char left   = IsKeyPressed(SDLK_LEFT);
	char right  = IsKeyPressed(SDLK_RIGHT);
	bool accept = IsKeyPressed(SDLK_RETURN);
	g_SettingsSelect = Wrap(g_SettingsSelect + input, 1, 6);

	if (input != 0)
		PlaySoundVol(SND_CURSOR, 20);
	if (left || right || accept)
		PlaySoundVol(SND_CONFIRM, 20);


	switch (g_SettingsSelect)
	{
		case 1:
			if (left || right || accept)
			{
				settings->fullscreen = !settings->fullscreen;
				SDL_SetWindowFullscreen(window, settings->fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
				ModScreen(window, settings, 0, 0);
			}
			break;
		case 2:
			if (left || right)
			{
				ModScreen(window, settings, right-left, 0);
			}
			break;
		case 3:
			if (left || right)
			{
				ModScreen(window, settings, 0, right-left);
			}
			break;
		case 4:
			if (left || right)
			{
				settings->vsync = !settings->vsync;
				SDL_RenderSetVSync(renderer, settings->vsync);
			}
			break;
		case 5:
			if (left || right)
			{
				char val = (right-left) * 5;
				settings->volume = Clamp(settings->volume + val, 0, 100);
			}
		case 6:
			if (accept)
			{
				ResetSettings(settings);
				SDL_SetWindowFullscreen(window, settings->fullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
				SDL_RenderSetVSync(renderer, settings->vsync);
				ModScreen(window, settings, 0, 0);
			}
			break;
	}
}

void DrawSettings(SDL_Renderer *renderer, Settings *settings)
{
	DrawText(renderer, "^2C",       112, 16);
	DrawText(renderer, "^5INVADERS", 88, 32);
	DrawText(renderer, "^3SETTINGS", 88, 48);

	DrawTextRGBA(renderer, "FULLSCREEN         %s",     16,  64, g_SettingsSelect == 1 ? 0xFF0000FF : 0x808080FF, settings->fullscreen ? "YES" : "NO");
	DrawTextRGBA(renderer, "SCALING            %d",     16,  80, g_SettingsSelect == 2 ? 0xFF0000FF : 0x808080FF, settings->scale);
	DrawTextRGBA(renderer, "ORIENTATION        %d DEG", 16,  96, g_SettingsSelect == 3 ? 0xFF0000FF : 0x808080FF, settings->angle * 90);
	DrawTextRGBA(renderer, "VSYNC              %s",     16, 112, g_SettingsSelect == 4 ? 0xFF0000FF : 0x808080FF, settings->vsync ? "YES" : "NO");
	DrawTextRGBA(renderer, "VOLUME             %d",     16, 128, g_SettingsSelect == 5 ? 0xFF0000FF : 0x808080FF, settings->volume);
	DrawTextRGBA(renderer, "RESET ALL"           ,      16, 240, g_SettingsSelect == 6 ? 0xFF0000FF : 0x808080FF);
}


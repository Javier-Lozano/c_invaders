#include "scene.h"

#define TITLE_TOP 32

typedef enum {
	TITLE_START,
	TITLE_SETTINGS,
	TITLE_EXIT
} TitleOptions;

///// Functions

static void SettingsScreen();

///// Title Screen

void Scene_Title()
{
	// Variables
	SDL_Event event;

	// Get Title screen texture rect
	SDL_Rect title_rect = {20, GAME_H, 0, 0};
	SDL_QueryTexture(g_Resources.textures[TEXTURE_TITLE], NULL, NULL, &title_rect.w, &title_rect.h);

	// Setup Pivot 
	TitleOptions select = TITLE_START;
	int options_pos_y[] = { 168, 184,200 };
	SDL_Color options_colors[3] = {g_Colors[C_YELLOW]};

	while(true)
	{
		// Event
		while(SDL_PollEvent(&event))
		{
			// Exit Game
			if (event.type == SDL_QUIT)
			{
				g_GameScene = SCENE_EXIT;
				return;
			}

			// Keyboard
			if (event.type == SDL_KEYDOWN)
			{
				if (title_rect.y != TITLE_TOP)
				{
					title_rect.y = TITLE_TOP;
				}
				else if (title_rect.y == TITLE_TOP)
				{
					if (event.key.keysym.scancode == SDL_SCANCODE_UP)	{ select--; }
					if (event.key.keysym.scancode == SDL_SCANCODE_DOWN)	{ select++; }
					if (event.key.keysym.scancode == SDL_SCANCODE_RETURN)
					{
						switch(select)
						{
							case TITLE_START:
								g_GameScene = SCENE_PLAY;
								return;
							case TITLE_SETTINGS:
								SettingsScreen();
								break;
							case TITLE_EXIT:
								g_GameScene = SCENE_EXIT;
								return;
						}
					}
				}
			}
		}

		// Update

		// Title Texture Animation
		if (title_rect.y > TITLE_TOP)
		{
			title_rect.y--;
		}

		// Pivot
		if (select < TITLE_START) { select = TITLE_EXIT; }
		if (select > TITLE_EXIT) { select = TITLE_START; }

		// Pivot Text Colors
		for(int i = 0; i < 3; i++)
		{
			if (i == select) { options_colors[i] = g_Colors[C_YELLOW]; }
			else { options_colors[i] = g_Colors[C_WHITE]; }
		}

		// Render

		// Prepare Rendering
		Rendering_Prepare();

		// Draw Background
		Scene_Background();

		// Render Title
		SDL_RenderCopy(g_Renderer, g_Resources.textures[TEXTURE_TITLE], NULL, &title_rect);

		// Show text on TITLE_MAIN
		if (title_rect.y == TITLE_TOP)
		{
			FontPrint(g_Renderer, g_Resources.font, "PLAYER",	8, 8, &g_Colors[C_RED]);
			FontPrint(g_Renderer, g_Resources.font, "HI-SCORE",	GAME_W - 72, 8, &g_Colors[C_GREEN]);

			FontPrint(g_Renderer, g_Resources.font, "START",	(GAME_W / 2), 168, &options_colors[0]);
			FontPrint(g_Renderer, g_Resources.font, "SETTINGS",	(GAME_W / 2), 184, &options_colors[1]);
			FontPrint(g_Renderer, g_Resources.font, "EXIT",		(GAME_W / 2), 200, &options_colors[2]);

			FontPrint(g_Renderer, g_Resources.font, "*BY JAVIER LOZANO*", 48, GAME_H - 16, &g_Colors[C_MAGENTA]);

			// Test Pivot
			SDL_SetRenderDrawColor(g_Renderer, 0xFF, 0xFF, 0, 0xFF);
			SDL_RenderFillRect(g_Renderer, &((SDL_Rect) {(GAME_W / 2) - 16, options_pos_y[select], 8, 8 }));
		}

		//SDL_RenderDrawLine(g_Renderer, GAME_W / 2, 0, GAME_W / 2, GAME_H);

		// Render Game
		Rendering_Present();
	}
}

static void SettingsScreen()
{
	printf("Settings!\n");
}

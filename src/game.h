#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>
#include "SDL.h"

typedef struct Settings {
	char scale;
	char angle;
	bool vsync;
	bool fullscreen;
} Settings;

typedef struct GameContext {
	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *fbuffer;

	Settings settings;

	double accumulator;
	double elapsed_time;

	bool is_running;
} GameContext;

void Events(GameContext *game);
void ClearScreen(GameContext *game);
void DrawScreen(GameContext *game);

void LoadSaveFile(GameContext *game);
void WriteSaveFile(GameContext *game);

#endif // GAME_H_


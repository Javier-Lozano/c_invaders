#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>
#include "SDL.h"

struct GameContext;

typedef struct Scene {
	void (*init)(struct GameContext *game);
	void (*update)(struct GameContext *game);
	void (*fixed_update)(struct GameContext *game);
	void (*draw)(struct GameContext *game);
	bool is_starting;
} Scene;

typedef struct Settings {
	int  highscore[10];
	char names[10][16];
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
	Scene    scene;

	double accumulator;
	double elapsed_time;

	bool is_running;
} GameContext;

void GameUpdate(GameContext *game);
void LoadSaveFile(GameContext *game);
void WriteSaveFile(GameContext *game);

Scene SetSceneTitle();

#endif // GAME_H_


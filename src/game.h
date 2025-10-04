#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>
#include "scene.h"

struct SDL_Renderer;
struct SDL_Window;
struct SDL_Texture;

typedef struct Settings {
	int  highscore[10];
	char names[10][10];
	char scale;
	char angle;
	bool vsync;
	bool fullscreen;
} Settings;

typedef struct GameContext {
	struct SDL_Window   *window;
	struct SDL_Renderer *renderer;
	struct SDL_Texture  *fbuffer;

	SceneID  scene_id;
	Settings settings;

	double accumulator;
	double elapsed_time;

	bool is_running;
} GameContext;

void GameLoop(GameContext *game);
void LoadSAVEDAT(GameContext *game);
void WriteSAVEDAT(GameContext *game);
void ModScreen(GameContext *game, char s_value, char a_value);

#endif // GAME_H_


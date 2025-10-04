#ifndef GAME_H_
#define GAME_H_

#include <stdbool.h>
#include "scene.h"
#include "settings.h"

struct SDL_Renderer;
struct SDL_Window;
struct SDL_Texture;

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

#endif // GAME_H_


#ifndef SCENE_H_
#define SCENE_H_

#include <stdbool.h>

typedef enum {
	SCENE_TITLE,
	SCENE_PLAY
} SceneID;

struct GameContext;
struct Settings;
struct SDL_Renderer;

typedef struct Scene {
	void (*init)(struct GameContext *game);
	void (*update)(struct GameContext *game);
	void (*fixed_update)(struct GameContext *game);
	void (*draw)(struct GameContext *game);
	bool is_starting;
} Scene;

void UpdateScene(struct GameContext *game);
void SetScene(SceneID id);

Scene GetSceneTitle(); // Defined at 's_title.c'
Scene GetScenePlay();  // Defined at 's_play.c'

void UpdateSettings(struct GameContext *game); // Defined at 'sub_settings.c'
void DrawSettings(struct SDL_Renderer *renderer, struct Settings *settings);   // Defined at 'sub_settings.c'

#endif // SCENE_H_


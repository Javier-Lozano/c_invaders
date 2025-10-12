#ifndef INVADERS_H_
#define INVADERS_H_

#include <stdbool.h>
#include "SDL.h"
#include "graphics.h"

///// Macros

#define DEFAULT_FPS (60)
#define WINDOW_W    (240)
#define WINDOW_H    (320)

#define ASSERT(assert, str) \
	do { \
		if (!(assert)) { \
			printf("\033[1;91m ASSERT:\033[0m '"#assert"' failed. '%s' at line %d\033.\n", __FILE__, __LINE__); \
			printf("\033[1;93mMESSAGE:\033[0m '%s'\n", str); \
			exit(EXIT_FAILURE); \
		} \
	} while(0)

#define LERP(a, b, t) (((1 - t) * a) + t * b)
#define WRAP(val, min, max) ((val < min) ? max : ((val > max) ? min : val))
#define CLAMP(val, min, max) ((val < min) ? min : ((val > max) ? max : val))
#define MIN(v1, v2) (v1 < v2 ? v1 : v2)
#define MAX(v1, v2) (v1 > v2 ? v1 : v2)

///// Input

typedef enum {
	STATE_UP       = 0,
	STATE_DOWN     = 1,
	STATE_PRESSED  = 2,
	STATE_RELEASED = 4
} ButtonState;

void InputUpdate();
void InputClear();
bool CheckState(SDL_Scancode scancode, ButtonState state);
bool IsAnyKeyPressed();
bool IsKeyDown(SDL_Keycode key);
bool IsKeyPressed(SDL_Keycode key);
bool IsKeyReleased(SDL_Keycode key);

///// Transition

typedef enum {
	TRANS_NONE,
	TRANS_INIT,
	TRANS_WAIT,
	TRANS_END
} TransState;

void StartTransition(int stage);
TransState GetTransitionState();

///// Settings

typedef struct Settings {
	int  highscore[10];
	char names[10][11];
	char scale;
	char angle;
	bool vsync;
	bool fullscreen;
} Settings;

void LoadSAVEDAT(Settings *settings);
void WriteSAVEDAT(Settings *settings);
void ModScreen(struct SDL_Window *window, Settings *s, char s_value, char a_value);
void UpdateSettings(SDL_Window *window, SDL_Renderer *renderer, Settings *settings);
void DrawSettings(SDL_Renderer *renderer, Settings *settings);

///// Scene

typedef enum {
	SCENE_TITLE,
	SCENE_PLAY,
	//SCENE_CREDITS,
	//SCENE_REPLAY,
} SceneID;

struct GameContext;

typedef struct Scene {
	void (*init)        ();
	void (*update)      (struct GameContext *game, double dt);
	void (*fixed_update)(struct GameContext *game, double fixed_dt);
	void (*draw)        (struct GameContext *game, double dt, double alpha);
	bool is_starting;
} Scene;

void SwitchScene(SceneID id);

Scene GetSceneTitle(); // Defined at 's_title.c'
Scene GetScenePlay();  // Defined at 's_play.c'

///// GameContext

typedef struct GameContext {
	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *fbuffer;

	Settings settings;
	int      framerate;
	bool     is_running;
} GameContext;

void InitInvaders(GameContext *game, int argc, char *argv[]);
void EndInvaders(GameContext *game);
void GameLoop(GameContext *game);

#endif // INVADERS_H_

#ifndef COMMON_H_
#define COMMON_H_

#include "SDL.h"
#include "SDL_mixer.h"
#include <stdbool.h>
#include "input.h"
#include "graphics.h"

///// MACROS

#define FPS (60)
#define WINDOW_W (240)
#define WINDOW_H (320)

#define ASSERT(assert, msg) \
	do { \
		if (!(bool)(assert)) { \
			printf("ASSERT: '"#assert"' failed. '%s' line %d.\nMESSAGE: '%s'", __FILE__, __LINE__, msg); \
			exit(EXIT_FAILURE); \
		} \
	} while(0) \

#define INTERPOLATE(a, b, t) (((1 - t) * a) + t * b)
#define CLAMP_WRAP(val, min, max) ((val < min) ? max : ((val > max) ? min : val))
#define CLAMP(val, min, max) ((val < min) ? min : ((val > max) ? max : val))

typedef enum {
	TRANS_NONE,
	TRANS_IN,
	TRANS_WAIT,
	TRANS_OUT,
} TransitionState;

typedef struct GameContext{
	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *buffer;

	struct Scene {
		void (*init)(struct GameContext *game);
		void (*update)(struct GameContext *game);
		void (*fixed_update)(struct GameContext *game);
		void (*draw)(struct GameContext *game);
		bool is_starting;
	} scene;

	struct Transition {
		TransitionState state;
		float offset;
		float timer;
		int stage;
	} transition;

	double accumulator;
	double elapsed_time;
	int highscore;
	bool is_running;
} GameContext;

typedef struct Scene Scene;
typedef struct Transition Transition;

Scene SetSceneTitle();
Scene SetScenePlay();

void StartTransition(GameContext *game, float wait_timer, int stage);
void UpdateTransition(GameContext *game);

#endif // COMMON_H_

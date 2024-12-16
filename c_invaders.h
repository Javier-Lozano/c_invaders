#ifndef C_INVADERS_H_
#define C_INVADERS_H_

#include "SDL.h"
#include "SDL_mixer.h"

/***** Enums *****/

typedef enum {
	SCENE_TITLE = 0,
	SCENE_PLAY,
	SCENE_CREDITS
} Scene;

/***** Structs *****/

typedef struct game_context_st {
	int is_running;
	Scene scene;
	char username[8];
	Uint32 hiscore;
} GameContext;

#endif // C_INVADERS_H_

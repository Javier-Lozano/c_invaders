#ifndef C_INVADERS_H_
#define C_INVADERS_H_

#include <stdbool.h>

#include "SDL.h"
#include "SDL_mixer.h"
#include "common_types.h"
#include "utils.h"
#include "input.h"

#define WINDOW_W (240)
#define WINDOW_H (320)

//typedef struct {
//	bool fullscreen; // false;
//	bool vsync;      // false;
//	bool show_fps;   // true;
//	u8   scale;      // 2;
//	i8   volume;     // 25; // Range from 0 to 100
//	u32  score;      // 0;
//	u32  hi_score;   // 37500; // Old Konami's default hi-score
//} Settings;

typedef struct game_context_st {
	// SDL
	SDL_Window   *window;
	SDL_Renderer *renderer;
//	SDL_Texture  *target;

	// Score
	char score_name[10][9];
	u32 score[10];

	// Control
	bool is_running;
	f64  fixed_framerate;
	f64  accumulator;
	FuncScene current_screen;
} GameContext;

void ScreenTitle(GameContext *game);
void ScreenPlay(GameContext *game);

#endif // C_INVADERS_H_

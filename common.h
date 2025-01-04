#ifndef C_INVADERS_H_
#define C_INVADERS_H_

#include "common_types.h"
#include "utils.h"

#define WINDOW_W (240)
#define WINDOW_H (320)

typedef enum {
	SCREEN_TITLE,
	SCREEN_CONFIG,
	SCREEN_PLAY
} Screen;

typedef struct {
	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *target;
} SDLContext;

typedef struct {
	bool fullscreen; // false;
	bool vsync;      // false;
	bool show_fps;   // true;
	u8   scale;      // 2;
	i8   volume;     // 25; // Range from 0 to 100
	u32  score;      // 0;
	u32  hi_score;   // 37500; // Old Konami's default hi-score
} Settings;

typedef struct game_context_st {
	SDLContext sdl;
	Settings   settings;
	bool       is_running;
	Screen     screen;
} GameContext;

#endif // C_INVADERS_H_

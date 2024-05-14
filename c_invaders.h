#ifndef _C_INVADERS_H_
#define _C_INVADERS_H_

#include <SDL2/SDL.h>
#include <stdbool.h>

///// MACROS

#define WIN_W 240
#define WIN_H 320

#define SCALE 2

#define INPUT_CHECK(input, value) (input & (1 << value))
#define INPUT_SET(input, value)   (input | (1 << value))

///// Enums

enum {
	INPUT_UP,
	INPUT_DOWN,
	INPUT_LEFT,
	INPUT_RIGHT,
	INPUT_FIRE,
	INPUT_CANCEL,
	INPUT_COUNT
};

enum {
	SCENE_INTRO,
	SCENE_PLAY,
	SCENE_CREDITS
};

///// Types

typedef struct {
	SDL_Texture *texture;
	SDL_Rect    rect;
	Uint8       scale;
} Target;

typedef struct {
	// SDL
	SDL_Window   *window;
	SDL_Renderer *renderer;
	// Target
	Target       target;
	// Scene
	Uint8        scene;
	// Input
	SDL_Scancode keybindings[INPUT_COUNT];
	// Control
	bool         is_running;
	double       delta_time;
} GameState;

///// Functions

// init.c
int Game_Init(GameState *game_state);
void Game_Close(GameState *game_state);

// events.c
void Game_HandleEvents(GameState *game_state);

// scene_*.c
void Scene_Intro(GameState *game_state);

#endif // _C_INVADERS_H_


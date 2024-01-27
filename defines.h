#ifndef _DEFINES_H_
#define _DEFINES_H_

// SDL2 Flags
#define SDL_INIT_FLAGS		SDL_INIT_VIDEO
#define SDL_WINDOW_FLAGS	SDL_WINDOW_SHOWN
#define SDL_RENDER_FLAGS	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

// Game Specific Macros
#define GAME_W	240
#define GAME_H	320

// Bit Manipulation for Input
#define SET_BIT(data, shift)	data | (1 << shift)
#define UNSET_BIT(data, shift)	data & ~(1 << shift)
#define CHECK_BIT(data, shift)	data & (1 << shift)
#define CHECK_BIT_BOOL(data, shift) (data & (1 << shift)) >> shift

// Player Macros
#define PLAYER_W 16
#define PLAYER_H 8
#define PLAYER_X ((GAME_W / 2) - PLAYER_W)
#define PLAYER_Y (GAME_H - 32)
#define PLAYER_SPD_X 2

// Bullet Macros
#define BULLET_MAX 100
#define PLAYER_BULLET 0

#endif // _DEFINES_H_


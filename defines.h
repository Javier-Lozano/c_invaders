#ifndef _DEFINES_H_
#define _DEFINES_H_

// SDL2 Flags
#define SDL_INIT_FLAGS		SDL_INIT_VIDEO
#define SDL_WINDOW_FLAGS	SDL_WINDOW_SHOWN
#define SDL_RENDER_FLAGS	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC

// Game Macros
#define GAME_W	240
#define GAME_H	320
#define GAME_BORDER 16

// Input
#define SET_BIT(data, shift)	data | (1 << shift)
#define UNSET_BIT(data, shift)	data & ~(1 << shift)
#define CHECK_BIT_BOOL(data, shift) ((data >> shift) & 1)

#define CHECK_INPUT(data, shift) CHECK_BIT_BOOL(data, shift)

// Player Macros
#define PLAYER_W 16
#define PLAYER_H 8
#define PLAYER_X ((GAME_W / 2) - PLAYER_W)
#define PLAYER_Y (GAME_H - 32)
#define PLAYER_SPD_X 2

// Swarm Macros
#define ENEMY_W 12
#define ENEMY_H 8
#define ENEMY_GAP 4
#define ENEMY_SPEED 2
#define ENEMY_STARTING_Y 64
#define SWARM_COLS 11
#define SWARM_ROWS 5
#define SWARM_COUNT SWARM_COLS * SWARM_ROWS

// Bullet Macros
#define BULLET_W 2
#define BULLET_H 6
#define BULLET_COUNT SWARM_COUNT + 1

// Check Borders
#define CHECK_X_BORDER(a, b) (a < 0 || b > GAME_W)
#define CHECK_Y_BORDER(a, b) (a < GAME_BORDER || b > (GAME_H - GAME_BORDER))
#define CHECK_BORDERS(rect) CHECK_X_BORDER(rect.x, rect.x + rect.w) || CHECK_Y_BORDER(rect.y, rect.y + rect.h)
#define CHECK_LEFTMOST(a, b) (a > b) ? a : b

// Check AABB Collision
#define CHECK_COLLISION(rect_1, rect_2)	(rect_1.x < (rect_2.x + rect_2.w) && (rect_1.x + rect_1.w) > rect_2.x && rect_1.y < (rect_2.y + rect_2.h) && (rect_1.y + rect_1.h) > rect_2.y)

#endif // _DEFINES_H_


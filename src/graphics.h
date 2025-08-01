#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "common.h"

typedef enum {
	SPR_PLAYER,
	SPR_ENEMY_A_1, SPR_ENEMY_A_2,
	SPR_ENEMY_B_1, SPR_ENEMY_B_2,
	SPR_ENEMY_C_1, SPR_ENEMY_C_2,
	SPR_ENEMY_UFO,
	SPR_PLAYER_DEAD_1, SPR_PLAYER_DEAD_2, SPR_PLAYER_DEAD_3, SPR_PLAYER_DEAD_4,
	SPR_HIT_1, SPR_HIT_2, SPR_HIT_3, SPR_HIT_4,
	SPR_BULLET_HIT_1, SPR_BULLET_HIT_2, SPR_BULLET_HIT_3, SPR_BULLET_HIT_4,
	SPR_BULLET_PLAYER,
	SPR_BULLET_A_1, SPR_BULLET_A_2, SPR_BULLET_A_3, SPR_BULLET_A_4,
	SPR_BULLET_B_1, SPR_BULLET_B_2, SPR_BULLET_B_3, SPR_BULLET_B_4,
	SPR_BULLET_C_1, SPR_BULLET_C_2, SPR_BULLET_C_3, SPR_BULLET_C_4,
	SPR_SHIELD_1, SPR_SHIELD_2, SPR_SHIELD_3,
	SPR_TILE_1, SPR_TILE_2, SPR_TILE_3, SPR_TILE_4, SPR_TILE_5,
	SPR_TILE_6, SPR_TILE_7, SPR_TILE_8, SPR_TILE_9, SPR_TILE_10,
	SPR_TILE_11, SPR_TILE_12, SPR_TILE_13,
	SPR_CURSOR,
	SPR_COUNT
} SpriteID;

typedef enum {
	SEQ_PLAYER_DEAD,
	SEQ_HIT,
	SEQ_BULLET_HIT,
	SEQ_BULLET_A,
	SEQ_BULLET_B,
	SEQ_BULLET_C,
	SEQ_COUNT
} SequenceID;

typedef struct {
	SequenceID seq_id;
	int pivot;
	float duration;
	float timer;
} Animation;

void InitGraphics(SDL_Renderer *renderer);
void CloseGraphics();

void SetGraphicsColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void DrawTile(SDL_Renderer *renderer, int tile, int x, int y);
void DrawTileRGBA(SDL_Renderer *renderer, int tile, int x, int y, unsigned int rgba);

void DrawSprite(SDL_Renderer *renderer, SpriteID sprite, int x, int y);
void DrawSpriteRGBA(SDL_Renderer *renderer, SpriteID sprite, int x, int y, unsigned int rgba);

void InitAnimation(Animation *anim, SequenceID seq_id, float seconds);
void PlayAnimation(SDL_Renderer *renderer, Animation *anim, float dt, int x, int y);

int DrawText(SDL_Renderer *renderer, const char *str, int x, int y, ...);
int DrawTextRGBA(SDL_Renderer *renderer, const char *str, int x, int y, unsigned int rgba, ...);

void DrawGraphicsTexture(SDL_Renderer *renderer);

#endif // GRAPHICS_H_

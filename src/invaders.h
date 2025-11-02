#ifndef INVADERS_H_
#define INVADERS_H_

#include <stdbool.h>
#include "SDL.h"

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

///// Audio

typedef enum {
	SND_CURSOR,
	SND_CONFIRM,
	SND_CANCEL,
	SND_FIRE_PLAYER,
	SND_FIRE_ALIEN,
	SND_HIT_UFO,
	SND_HIT_PLAYER,
	SND_HIT_MISS,
	SND_HIT_ALIEN,
	SND_STEP1,
	SND_STEP2,
	SND_STEP3,
	SND_STEP4,
	SND_GAME_OVER,
	SND_COUNT
} SoundID;

void PlaySoundVol(SoundID id, char volume);
void PlaySound(SoundID id);

///// Graphics

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
	SPR_TILE_1,  SPR_TILE_2,  SPR_TILE_3, SPR_TILE_4, SPR_TILE_5,
	SPR_TILE_6,  SPR_TILE_7,  SPR_TILE_8, SPR_TILE_9, SPR_TILE_10,
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

void SetGraphicsColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

void DrawTile(struct SDL_Renderer *renderer, int tile, int x, int y);
void DrawTileRGBA(struct SDL_Renderer *renderer, int tile, int x, int y, unsigned int rgba);

void DrawSprite(struct SDL_Renderer *renderer, SpriteID sprite, int x, int y);
void DrawSpriteRGBA(struct SDL_Renderer *renderer, SpriteID sprite, int x, int y, unsigned int rgba);

int DrawChar(struct SDL_Renderer *renderer, unsigned int c, int x, int y);
int DrawCharRGBA(struct SDL_Renderer *renderer, unsigned int c, int x, int y, unsigned int rgba);

int DrawText(struct SDL_Renderer *renderer, const char *str, int x, int y, ...);
int DrawTextRGBA(struct SDL_Renderer *renderer, const char *str, int x, int y, unsigned int rgba, ...);

void InitAnimation(Animation *anim, SequenceID seq_id, float seconds);
void PlayAnimation(struct SDL_Renderer *renderer, Animation *anim, float dt, int x, int y);

void DrawGraphicsTexture(struct SDL_Renderer *renderer);

///// Input

typedef enum {
	STATE_UP       = 0,
	STATE_DOWN     = 1,
	STATE_PRESSED  = 2,
	STATE_RELEASED = 4
} ButtonState;

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

///// Settings

typedef struct Settings {
	int  highscore[10];
	char names[10][11];
	char volume;
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

///// Game

typedef struct GameContext {
	SDL_Window   *window;
	SDL_Renderer *renderer;
	SDL_Texture  *fbuffer;

	Settings settings;
	int      framerate;
	bool     is_running;
} GameContext;

void InitGame(GameContext *game, int argc, char *argv[]);
void CloseGame(GameContext *game);
void GameLoop(GameContext *game);

#endif // INVADERS_H_

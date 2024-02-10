#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <SDL2/SDL.h>

enum EntityState {
	ENTITY_NONE,
	ENTITY_START,
	ENTITY_PLAY,
	ENTITY_END,
	ENTITY_ENEMY_DROP
};
typedef struct {
	SDL_Rect box;
	int speed;
	Uint8 state;
	Uint8 waitTime;
	//Sprite sprite;
} Entity;

#endif //_ENTITY_H_

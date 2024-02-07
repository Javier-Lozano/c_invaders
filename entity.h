#ifndef _ENTITY_H_
#define _ENTITY_H_

#include <SDL2/SDL.h>

enum EntityShifts
{

};

typedef struct {
	SDL_Rect box;
	SDL_Point speed;
	Uint8 state;
} Entity;

#endif //_ENTITY_H_

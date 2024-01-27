#ifndef _SWARM_H_
#define _SWARM_H_


typedef struct {
	SDL_Rect box;
	SDL_Point speed;
	int isActive;
} Alien;

typedef struct {
	Alien aliens[2];
	int alienCount;
} Swarm;



#endif // _SWARM_H_

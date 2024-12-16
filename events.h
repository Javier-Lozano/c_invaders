#ifndef EVENTS_H_
#define EVENTS_H_

#include "SDL.h"
#include "c_invaders.h"
#include "input.h"

/***** Functions *****/

void ProcessEvents(GameContext *ctx);
double GetTimeDelta();

#endif //EVENTS_H_

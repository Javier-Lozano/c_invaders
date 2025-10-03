#ifndef TRANSITION_H_
#define TRANSITION_H_

struct SDL_Renderer;

typedef enum {
	TRANS_NONE,
	TRANS_INIT,
	TRANS_WAIT,
	TRANS_END,
} TransState;

void UpdateTransition(struct SDL_Renderer *renderer, double dt);
void StartTransition(int stage);
TransState GetTransitionState();

#endif // TRANSITION_H_


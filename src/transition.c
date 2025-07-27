#include "common.h"

void StartTransition(GameContext *game, float wait_timer, int stage)
{
	game->transition.state  = TRANS_IN;
	game->transition.timer  = wait_timer;
	game->transition.offset = 0.0f;
	game->transition.stage  = stage;
}

void UpdateTransition(GameContext *game)
{
	const float half = WINDOW_H / 2;
	const float dt   = game->elapsed_time;
	Transition *t = &game->transition;

	switch(t->state)
	{
		case TRANS_NONE:
			return;

		case TRANS_IN:
			t->offset += dt * 320;
			if (t->offset > half)
			{
				t->offset = half;
				t->state = TRANS_WAIT;
			}
			break;
		case TRANS_WAIT:
			t->timer -= dt;
			if (t->timer < 0)
				t->state = TRANS_OUT;
			break;
		case TRANS_OUT:
			t->offset -= dt * 320;
			if (t->offset < 0)
				t->state = TRANS_NONE;
			break;
	}

	SDL_SetRenderDrawColor(game->renderer, 8, 8, 8, 0xFF);
	SDL_RenderFillRectF(game->renderer, &(SDL_FRect){0,        0, WINDOW_W,  t->offset});
	SDL_RenderFillRectF(game->renderer, &(SDL_FRect){0, WINDOW_H, WINDOW_W, -t->offset});
	if (t->stage > 0)
	{
		DrawText(game->renderer, "^3STAGE ^1%d^7", (WINDOW_W / 2) - 40, t->offset - 16, t->stage);
	}
}

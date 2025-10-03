#include <stdbool.h>
#include "SDL.h"
#include "transition.h"
#include "graphics.h"
#include "macros.h"

TransState g_State;
float      g_Offset;
int        g_Stage;

void UpdateTransition(SDL_Renderer *renderer, double dt)
{
	const float speed  = dt * INNER_FPS * 4;
	const bool  has_stage = g_Stage > 0;
	const float mid_point = WINDOW_H / 2;

	switch (g_State)
	{
		case TRANS_NONE:
			break;
		case TRANS_INIT:
			{
				g_Offset += speed;

				SDL_FRect rects[2] = {
					{0,        0, WINDOW_W,  g_Offset},
					{0, WINDOW_H, WINDOW_W, -g_Offset}
				};

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRectsF(renderer, rects, 2);

				if (g_Offset > mid_point)
				{
					g_Offset = 0;
					g_State  = TRANS_WAIT; 
				}

				if (has_stage)
					DrawText(renderer, "^1STAGE^7 %d", 84, g_Offset-8, g_Stage);
			}
			break;
		case TRANS_WAIT:
			{
				g_Offset += dt;

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderClear(renderer);

				if (g_Offset > 2)
				{
					g_Offset = mid_point;
					g_State  = TRANS_END; 
				}

				if (has_stage)
					DrawText(renderer, "^1STAGE^7 %d", 84, mid_point-8, g_Stage);
			}
			break;
		case TRANS_END:
			{
				g_Offset -= speed;

				SDL_FRect rects[2] = {
					{0,        0, WINDOW_W,  g_Offset},
					{0, WINDOW_H, WINDOW_W, -g_Offset}
				};

				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				SDL_RenderFillRectsF(renderer, rects, 2);

				if (g_Offset < 0)
					g_State = TRANS_NONE; 

				if (has_stage)
					DrawText(renderer, "^1STAGE^7 %d", 84, g_Offset-8, g_Stage);
			}
			break;
	}
}

void StartTransition(int stage)
{
	g_State  = TRANS_INIT;
	g_Offset = 0.0f;
	g_Stage  = stage;
}

TransState GetTransitionState()
{
	return g_State;
}


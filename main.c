#include "c_invaders.h"
#include "rendering.h"
#include "assets.h"
#include "events.h"
#include "input.h"

/***** Functions (Prototypes) *****/

void SceneTitle(GameContext *ctx);

/***** Main *****/

int main(int argc, char *argv[])
{
	GameContext ctx;

	if (!InitSDL() || !InitAssets())
	{
		return 0;
	}

	ctx.is_running = 1;
	ctx.scene      = SCENE_TITLE;
	ctx.hiscore    = 37500;

	while(ctx.is_running)
	{
		switch(ctx.scene)
		{
			case SCENE_TITLE:
				SceneTitle(&ctx);
				break;
		}
	}

	CloseAssets();
	CloseSDL();

	printf("\n\x1B[97;1mSEE YOU SPACE COWBOY...\x1B[0m\n\n");
	return 0;
}

/***** Functions *****/

void SceneTitle(GameContext *ctx)
{
	const Uint32 colors [] = {
		0xFF0000FF, 0x00FF00FF, 0x0000FFFF, 0xFFFFFFFF
	};
	const Uint8 tilemap_c[] = {
		   0,   0,0x73,0x70,0x70,0x74,   0,   0,
		   0,0x73,0x72,0x77,0x77,0x71,0x74,   0,
		0x73,0x72,0x73,0x70,0x70,0x74,0x71,0x74,
		0x70,0x75,0x70,0x7A,0x79,0x7A,   0,0x70,
		0x70,0x75,0x70,0x7C,0x7B,0x7C,   0,0x70,
		0x71,0x74,0x71,0x70,0x70,0x72,0x73,0x72,
		   0,0x71,0x74,0x78,0x78,0x73,0x72,   0,
		   0,   0,0x71,0x70,0x70,0x72,   0,   0,
	};
	const Uint8 tilemap_invaders[] = {
		0x70,0x70,0x75,0x76,0x75,0x76,0x70,   0,0x70,0x7B,0x70,0x74,0x75,0x70,0x74,0x75,0x70,0x70,0x75,0x70,0x74,0x7B,0x70,0x74,
		0x75,0x76,0x75,0x70,0x73,0x76,0x70,   0,0x70,0x75,0x76,0x70,0x75,0x76,0x70,0x75,0x76,   0,0x75,0x76,0x70,0x75,0x76,0x79,
		0x75,0x76,0x75,0x70,0x70,0x76,0x70,0x77,0x70,0x75,0x70,0x70,0x75,0x76,0x70,0x75,0x70,0x76,0x75,0x70,0x74,0x79,0x70,0x74,
		0x75,0x76,0x75,0x76,0x71,0x76,0x75,0x70,0x76,0x75,0x76,0x70,0x75,0x76,0x70,0x75,0x76,   0,0x75,0x76,0x70,0x7B,   0,0x70,
		0x70,0x70,0x75,0x76,0x75,0x76,   0,0x70,   0,0x75,0x76,0x70,0x75,0x70,0x72,0x75,0x70,0x70,0x75,0x76,0x70,0x79,0x70,0x72,
	};
	const int cols = WINDOW_W / SPRITE_SIZE;
	const int rows = WINDOW_H / SPRITE_SIZE;
	const double step = 1.0f / 30.0f;

	double time = 0.0f;
	int tile = 0;
	Uint8 state = 0;
	int sel = 0;

	while(ctx->is_running)
	{
		// Events
		ProcessEvents(ctx);

		switch (state)
		{
			case 0:
				{
					time += GetTimeDelta();
					if (time >= step)
					{
						time -= step;
						tile += 1;
					}
					if (tile > 68)
					{
						state = 1;
					}
					break;
				}
			case 1:
				{
				}
			default:
				break;
		}

		// Prepare
		SDL_SetRenderDrawColor(GetRenderer(), 0, 0, 0, 0xFF);
		SDL_RenderClear(GetRenderer());

		// Text
		if (state > 0)
		{
			DrawText("%s", 16, 8, 0x00FFFFFF, "PLAYER");
			DrawText("%d", 16, 24, 0xFFFFFFFF, 0);
			DrawText("HI-Score", 88, 8, 0xFFFF00FF);
			DrawText("%d", 88, 24, 0xFFFF00FF, ctx->hiscore);

			DrawText("START",     104, 200, (sel == 0) ? 0xFFFF00FF : 0xFFFFFFFF);
			DrawText("CONFIGURE", 104, 216, (sel == 1) ? 0xFFFF00FF : 0xFFFFFFFF);
			DrawText("EXIT",      104, 232, (sel == 2) ? 0xFFFF00FF : 0xFFFFFFFF);

			DrawText("PROGRAMMING", 16, 256, 0xFF0000FF);
			DrawText("* HIGHWIND *", 16, 272, 0x00FFFFFF);
			DrawText("SOUND", 16, 288, 0xFF0000FF);
			//DrawText("* SHIRU * (NESSFX)", 16, 304, 0x00FFFFFF);

			if(IsKeyPressed(SDLK_RETURN) && sel == 2) { ctx->is_running = 0; }
			if(IsKeyPressed(SDLK_UP))   { sel -= 1; }
			if(IsKeyPressed(SDLK_DOWN)) { sel += 1; }
			if (sel < 0) { sel = 2; }
			if (sel > 2) { sel = 0; }
		}

		// Tilemap C
		for (int i = 0; i < 64; i++)
		{
			int x = ((i % 8) * SPRITE_SIZE) + 88;
			int y = ((i / 8) * SPRITE_SIZE) + 64;
			DrawSprite(tilemap_c[i], x, y);
		}
		// Tilemap Invaders
		for (int i = 0; i < 120; i++)
		{
			int x = ((i % 24) * SPRITE_SIZE) + 24;
			int y = ((i / 24) * SPRITE_SIZE) + 144;
			DrawSprite(tilemap_invaders[i], x, y);
		}

		// Color Tilemap
		SDL_SetRenderDrawBlendMode(GetRenderer(), SDL_BLENDMODE_MUL);
		SDL_SetRenderDrawColor(GetRenderer(), 0, 0xFF, 0, 0xFF);
		SDL_RenderFillRect(GetRenderer(), &(SDL_Rect){88, 64, 64, 32});
		SDL_SetRenderDrawColor(GetRenderer(), 0, 0xFF, 0xFF, 0xFF);
		SDL_RenderFillRect(GetRenderer(), &(SDL_Rect){88, 96, 64, 32});
		SDL_SetRenderDrawColor(GetRenderer(), 0xFF, 0, 0xFF, 0xFF);
		SDL_RenderFillRect(GetRenderer(), &(SDL_Rect){24, 144, 192, 20});
		SDL_SetRenderDrawColor(GetRenderer(), 0xFF, 0xFF, 0, 0xFF);
		SDL_RenderFillRect(GetRenderer(), &(SDL_Rect){24, 164, 192, 20});
		SDL_SetRenderDrawBlendMode(GetRenderer(), SDL_BLENDMODE_BLEND);

		// Draw Curtain
		for (int y = 0, t = tile; state == 0 && y < rows; y++)
		{
			for (int x = t; x < cols; x++)
			{
				DrawSprite(0x70, x * SPRITE_SIZE, y * SPRITE_SIZE);
				//printf("Tiles: %d\n", tile)
			}
			if (t > 0) { t--; }
		}

		// Present
		SDL_RenderPresent(GetRenderer());
	}
}


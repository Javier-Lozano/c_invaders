#include <SDL2/SDL.h>
#include "defines.h"
#include "input.h"
#include "player.h"
//#include "swarm.h"
#include "bullet.h"

///// Globals /////

Uint8 g_Input;
SDL_Scancode g_Bindings[INPUT_SIZE];

///// Functions /////

void DrawBackground(SDL_Renderer *renderer);

int main(int argc, char **argv)
{
	///// Init. SDL /////

	SDL_Window *window	= NULL;
	SDL_Renderer *renderer	= NULL;
	SDL_Event event;
	int loop = 1;

	SDL_Init(SDL_INIT_FLAGS);
	window = SDL_CreateWindow("C Invaders", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GAME_W, GAME_H, SDL_WINDOW_FLAGS);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDER_FLAGS);

	///// Init. Game /////

	// Key Bindings
	g_Bindings[INPUT_LEFT]	= SDL_SCANCODE_LEFT;
	g_Bindings[INPUT_RIGHT]	= SDL_SCANCODE_RIGHT;
	g_Bindings[INPUT_FIRE]	= SDL_SCANCODE_SPACE;
	g_Bindings[INPUT_PAUSE]	= SDL_SCANCODE_ESCAPE;

	// Player
	Player player;
	InitPlayer(&player);
	printf("Player -> x: %d, y: %d, w: %d, h: %d \n", player.box.x, player.box.y, player.box.h, player.box.w);

	// Swarm
	//Swarm swarm;
	//InitSwarm(&swarm);

	// Bullets
	Bullet bullets[BULLET_MAX] = {0};

	///// Main Loop /////

	while (loop)
	{
		///// Events /////
		while(SDL_PollEvent(&event))
		{
			// Exit
			if (event.type == SDL_QUIT) { loop = 0; }
			// Input
			HandleInput(&event);
		}

		///// Update /////
		
		// Player
		UpdatePlayer(&player, bullets);
		//UpdatePlayer(&player, &bullets);
		// Swarm
		//UpdateSwarm(&swarm, &player, &bullets);
		// Bullets
		UpdateBullets(bullets);

		///// Render /////

		// Background
		DrawBackground(renderer);
		// Player
		RenderPlayer(renderer, &player);
		// Swarm
		//RenderSwarm(&swarm);
		// Bullets
		RenderBullets(renderer, bullets);

		// Present to Screen
		SDL_RenderPresent(renderer);

		//fprintf(stderr, "Input: %x\r", g_Input);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}

void DrawBackground(SDL_Renderer * renderer)
{
	// Clear Screen
	SDL_SetRenderDrawColor(renderer, 0, 0 ,0, 0xFF);
	SDL_RenderClear(renderer);

	// Lines Color
	SDL_SetRenderDrawColor(renderer, 0xA0, 0xA0, 0xA0, 0xFF);
	// Top and Bottom Lines
	SDL_RenderDrawLine(renderer, 0, 16, GAME_W, 16);
	SDL_RenderDrawLine(renderer, 0, GAME_H - 16, GAME_W, GAME_H - 16);
}

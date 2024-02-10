#include <SDL2/SDL.h>
#include "defines.h"
#include "input.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"

///// Globals /////

//Uint8 g_GameState;
Uint8 g_Input;
int g_BulletCount;
SDL_Scancode g_Bindings[INPUT_SIZE];

///// Functions /////

void DrawBackground(SDL_Renderer *renderer);
//void CheckCollision(Player *player, Bullet *bulletss, Swarm *swarm);

///// Main /////

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

	// Init. Bullet Count
	g_BulletCount = 0;

	// Entities
	Entity entities[ENTITY_COUNT] = {0};	// All entities. Player index: 0, Enemies indexes: 1 to 55, UFO index: 56.
	Entity bullets[ENTITY_COUNT] = {0};	// All bullets. Same indexes as the previous.
	// Player
	InitPlayer(&entities[ENTITY_PLAYER]);
	// Enemy
	InitEnemies(entities);

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
		UpdatePlayer(&entities[ENTITY_PLAYER], bullets);
		// Swarm
		UpdateEnemies(entities);
		// Bullets
		UpdateBullets(bullets);

		///// Collision Detection /////

		//CheckCollision(&player, bulletss, &swarm);

		///// Render /////

		// Background
		DrawBackground(renderer);
		// Player
		RenderPlayer(renderer, &entities[ENTITY_PLAYER]);
		// Swarm
		RenderEnemies(renderer, entities);
		// Bullets
		RenderBullets(renderer, bullets);

		// Present to Screen
		SDL_RenderPresent(renderer);

		//fprintf(stderr, "Input: %x\r", g_Input);
		//fprintf(stderr, "Bullet Count: %d\n", g_BulletCount);
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
	SDL_RenderDrawLine(renderer, 0, GAME_BORDER, GAME_W, GAME_BORDER);
	SDL_RenderDrawLine(renderer, 0, GAME_H - GAME_BORDER, GAME_W, GAME_H - GAME_BORDER);
}

//void CheckCollision(Player *player, Bullet *bulletss, Swarm *swarm)
//{
//	// Player Bullet vs Swarm
//	for(int i = 0; bulletss[0].speed != 0 && i < swarm->enemyCount; i++)
//	{
//		if (CHECK_COLLISION(bulletss[0].box, swarm->enemy[i].box))
//		{
//			//printf("Enemy Touched %d\n", i);
//			swarm->enemyCount--;
//			for(int j = i; j < swarm->enemyCount; j++)
//			{
//				swarm->enemy[j] = swarm->enemy[j + 1];
//			}
//			bulletss[0].speed = 0;
//		}
//	}
//
//	// Bullet / Player vs Bullet
//	for(int i = 1;  i < BULLET_COUNT; i++)
//	{
//		// Vs Player Bullet
//		if (bulletss[0].speed != 0 && CHECK_COLLISION(bulletss[0].box, bulletss[i].box))
//		{
//			bulletss[0].speed = 0;
//			bulletss[i].speed = 0;
//		}
//
//		// Vs Player
//		if (CHECK_COLLISION(player->box, bulletss[i].box))
//		{
//			bulletss[i].speed = 0;
//		}
//	}
//}

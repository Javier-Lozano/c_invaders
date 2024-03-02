#include "scene.h"

typedef enum {
	PLAY_START,
	PLAY_GAME,
	PLAY_OVER
} PlayState;

void Scene_Play()
{
	PlayState state = PLAY_START;
	SDL_Event event;
//	while(true)
//	{
//		// Prepare Rendering
//		Rendering_Prepare();
//
//		// Draw Background
//		Scene_Background();
//
//		// Render Game
//		Rendering_Present();
//
//		///// PLAY START
//
//		// Drop Enemies
//
//		// Draw Lines
//
//		///// PLAY GAME
//
//		///// PLAY OVER
//
//		// Draw Blinking Text "GAME OVER"
//
//	}
//
	g_GameScene = SCENE_EXIT;
}

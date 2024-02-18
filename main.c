#include "main.h"

///// Globals

// SDL
SDL_Window	*g_Window;
SDL_Renderer	*g_Renderer;
SDL_Texture	*g_Target;
SDL_Rect	g_TargetRectSrc;
SDL_Rect	g_TargetRectDst;
SDL_Event	g_Event;

// Integer Scaling
int g_Scale;

// Game
bool g_MainLoop;
ResourceManager g_ResourceManager;
GameScene g_GameScene;

///// Main

int main(int argc, char *argv[])
{
	// Init. SDL2
	if (Game_SDL_Start())
	{
		// Init Game

		// Main Loop
		g_MainLoop = true;
		// Set Scale
		Game_ScaleTarget(GAME_DEFAULT_SCALE);

		// Load Resources
		//if (Game_LoadResources(g_ResourceManager) )
		//{
		//	fprinf("Coulnd,'t load resources!\n");
		//	Game_SDL_End();
		//	return 1;
		//}

		while(g_MainLoop)
		{
			// Check Game Scene
			switch(g_GameScene)
			{
				case SCENE_TITLE:
					Game_SceneTitle();
					break;
				case SCENE_PLAY:
					//Game_ScenePlay();
					break;
				case SCENE_CREDITS:
					//Game_SceneCredits();
					break;
			}
		}
	}

	fprintf(stdout, "SEE YOU SPACE COWBOY...\n");

	// Termina SDL2
	Game_SDL_End();

	return 0;
}

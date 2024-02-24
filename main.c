#include "main.h"

///// Globals

GameScene g_GameScene;

int main(int argc, char *arcv[])
{
	// Variables
	bool loop = true;

	// Init. Game
	if (Game_Init())
	{
		// Set Scale to Default
		Rendering_SetScale(GAME_DEFAULT_SCALE);
		//Rendering_ToggleFullscreen();

		while(loop)
		{
			// Check Game Scene
			switch(g_GameScene)
			{
				case SCENE_TITLE:
					Scene_Title();
					break;
				case SCENE_PLAY:
					break;
				case SCENE_CREDITS:
					break;
				case SCENE_EXIT:
					loop = false;
					break;
			}
		}
	}

	fprintf(stdout, "SEE YOU SPACE COWBOY...\n");

	Game_End();
	return 0;
}


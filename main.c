#include "c_invaders.h"

int main(int argc, char *argv[])
{
	GameState game_state = {0};

	// Init Game
	if (Game_Init(&game_state))
	{
		// Scene Loop
		while(game_state.is_running)
		{
			switch(game_state.scene)
			{
				case SCENE_INTRO:
					Scene_Intro(&game_state);
					break;
				case SCENE_PLAY:
					game_state.scene = SCENE_CREDITS;
					printf("Play\n");
					//Scene_Play(&game_state);
					break;
				case SCENE_CREDITS:
					printf("Creadits\n");
					game_state.is_running = 0;
					//Scene_Credits(&game_state);
					break;
			}
		}

		printf("SEE YOU SPACE COWBOY!\n");
	}

	Game_Close(&game_state);

	return 0;
}


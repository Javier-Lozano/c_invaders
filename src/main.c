#include "invaders.h"

int main(int argc, char *argv[])
{
	GameContext game = {0};

	InitInvaders(&game, argc, argv);

	while(game.is_running)
		GameLoop(&game);

	EndInvaders(&game);

	printf("\n\033[1;97mSEE YOU SPACE COWBOY...\033[0m\n\n");

	return 0;
}


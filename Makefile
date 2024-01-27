# ########################### #
# Program:	c_invaders    #
# Author:	Javier Lozano #
# ########################### #

FLAGS	= -Wall -std=c99 -pedantic

c_invaders: main.c input.c player.c bullet.c
	cc $(FLAGS) $^ -o $@ -lSDL2

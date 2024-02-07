# ########################### #
# Program:	c_invaders    #
# Author:	Javier Lozano #
# ########################### #

FLAGS	= -Wall -std=c99 -pedantic -g
#SRC	= main.c input.c player.c bullet.c swarm.c

c_invaders: main.c input.c player.c bullet.c swarm.c
	cc $(FLAGS) $^ -o $@ -lSDL2


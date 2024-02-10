# ########################### #
# Program:	c_invaders    #
# Author:	Javier Lozano #
# ########################### #

FLAGS	= -Wall -std=c99 -pedantic
#SRC	= main.c input.c player.c bullet.c swarm.c

all: c_invaders

debug: FLAGS += -g
debug: c_invaders

c_invaders: main.c input.c player.c bullet.c enemy.c
	cc $(FLAGS) $^ -o $@ -lSDL2


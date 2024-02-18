# ########################### #
# Program:	c_invaders    #
# Author:	Javier Lozano #
# ########################### #

FLAGS	= -Wall -std=c99 -pedantic
#SRC	= main.c input.c player.c bullet.c swarm.c

all: c_invaders

debug: FLAGS += -g
debug: c_invaders

c_invaders: main.o system.o scene_title.o scene_background.o
	cc $(FLAGS) $^ -o $@ -lSDL2

main.o:
	cc $(FLAGS) -c main.c

system.o:
	cc $(FLAGS) -c system.c

scene_title.o:
	cc $(FLAGS) -c scene_title.c

scene_background.o:
	cc $(FLAGS) -c scene_background.c

rm:
	rm *.o c_invaders
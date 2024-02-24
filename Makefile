# ########################### #
# Program:	c_invaders    #
# Author:	Javier Lozano #
# ########################### #

FLAGS	= -Wall -std=c99 -pedantic
#SRC	= main.c input.c player.c bullet.c swarm.c

all: c_invaders

debug: FLAGS += -g
debug: c_invaders

c_invaders: main.o init.o rendering.o resources.o scene_title.o scene_background.o
	cc $(FLAGS) $^ -o $@ -lSDL2

main.o: main.c main.h
	cc $(FLAGS) -c main.c

init.o: init.c init.h
	cc $(FLAGS) -c init.c

rendering.o: rendering.c rendering.h
	cc $(FLAGS) -c rendering.c

resources.o: resources.c resources.h
	cc $(FLAGS) -c resources.c

scene_title.o: scene_title.c scene.h
	cc $(FLAGS) -c scene_title.c

scene_background.o: scene_background.c scene.h
	cc $(FLAGS) -c scene_background.c

rm:
	rm *.o c_invaders

# ########################### #
# Program:	c_invaders    #
# Author:	Javier Lozano #
# ########################### #

FLAGS	= -Wall --std=c99 --pedantic
#SRC	= main.c input.c player.c bullet.c swarm.c

all: c_invaders

debug: FLAGS += -g -O0
debug: c_invaders

c_invaders: main.o init.o rendering.o resources.o font.o scene_title.o scene_play.o scene_background.o
	cc $(FLAGS) $^ -o $@ -lSDL2

main.o: main.c main.h
	cc $(FLAGS) -c main.c

init.o: init.c init.h
	cc $(FLAGS) -c init.c

rendering.o: rendering.c rendering.h
	cc $(FLAGS) -c rendering.c

resources.o: resources.c resources.h
	cc $(FLAGS) -c resources.c

font.o: font.c font.h
	cc $(FLAGS) -c font.c

scene_title.o: scene_title.c scene.h
	cc $(FLAGS) -c scene_title.c

scene_play.o: scene_play.c scene.h
	cc $(FLAGS) -c scene_play.c

scene_background.o: scene_background.c scene.h
	cc $(FLAGS) -c scene_background.c

clean:
	rm -f *.o
	rm -f c_invaders

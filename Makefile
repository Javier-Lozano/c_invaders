# ###################### #
# Project: C Invaders    #
#  Author: Javier Lozano #
# ###################### #

CC      := cc
OPT     := -Wall
I_FLAGS := `sdl2-config --cflags`
L_FLAGS := -lSDL2 -lSDL2_ttf
PROJECT := c_invaders

# Rules

all: $(PROJECT)

debug: OPT += -g
debug: $(PROJECT)

$(PROJECT): main.o rendering.o assets.o events.o input.o
	$(CC) $(OPT) $^ -o $@ $(L_FLAGS)

main.o: main.c c_invaders.h input.h
	$(CC) $(OPT) $(I_FLAGS) -c $<
rendering.o: rendering.c rendering.h
	$(CC) $(OPT) $(I_FLAGS) -c $<
assets.o: assets.c assets.h
	$(CC) $(OPT) $(I_FLAGS) -c $<
events.o: events.c events.h c_invaders.h
	$(CC) $(OPT) $(I_FLAGS) -c $<
input.o: input.c input.h
	$(CC) $(OPT) $(I_FLAGS) -c $<

# Clean

.PHONY: clean
clean:
	@rm -fv *.o
	@rm -fv $(PROJECT)


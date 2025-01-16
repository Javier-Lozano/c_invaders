# ###################### #
# Project: C Invaders    #
#  Author: Javier Lozano #
# ###################### #

CC      := cc
OPT     := -Wall -std=c99
I_FLAGS := `sdl2-config --cflags`
L_FLAGS := -lSDL2 -lSDL2_ttf -lSDL2_mixer
PROJECT := c_invaders

# Rules

all: $(PROJECT)

debug: OPT += -g
debug: $(PROJECT)

$(PROJECT): main.c assets.c input.c events.c screen_title.c screen_play.c player.c bullet.c
	$(CC) $(I_FLAGS) $(OPT) $^ -o $@ $(L_FLAGS)

# Clean

.PHONY: clean
clean:
	@rm -fv *.o
	@rm -fv $(PROJECT)


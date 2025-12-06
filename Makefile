# ###################### #
# Project: C Invaders    #
#  Author: Javier Lozano #
#          (Highwind)    #
# ###################### #

# Variables

CC  := cc
OPT := -Wall -std=c99 -pedantic
INC := $(shell sdl2-config --cflags)
LIB := -lSDL2
PRG := c_invaders
SRC := src/main.c src/invaders.c src/s_title.c src/s_play.c src/sub_settings.c
OBJ := $(SRC:.c=.o)

MAKEFLAGS += -j$(shell nproc)

# Rules

all: debug

release: OPT += -O3
release: $(PRG)
	@rm -fv *.o

debug: OPT += -ggdb -O0
debug: $(PRG)

tools: assets/pack_sprites assets/pack_sounds

# Game

$(PRG): $(OBJ)
	$(CC) $^ -o $@ $(LIB)

src/main.o: src/main.c src/invaders.h
	$(CC) $(OPT) $(INC) -c $< -o $@

src/invaders.o: src/invaders.c src/invaders.h
	$(CC) $(OPT) $(INC) -c $< -o $@

src/s_title.o: src/s_title.c src/invaders.h
	$(CC) $(OPT) $(INC) -c $< -o $@

src/s_play.o: src/s_play.c src/invaders.h
	$(CC) $(OPT) $(INC) -c $< -o $@

src/sub_settings.o: src/sub_settings.c src/invaders.h
	$(CC) $(OPT) $(INC) -c $< -o $@

# Tools

assets/pack_sprites: assets/pack_sprites.c
	$(CC) $(OPT) $(INC) $< -o $@ $(LIB)

assets/pack_sounds: assets/pack_sounds.c
	$(CC) $(OPT) $(INC) $< -o $@ $(LIB)

# Clean

clean:
	@rm -fv $(OBJ)
	@rm -fv $(PRG)
	@rm -fv assets/pack_sprites
	@rm -fv assets/pack_sounds

.PHONY: all debug release tools clean


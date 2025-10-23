# ###################### #
# Project: C Invaders    #
#  Author: Javier Lozano #
#          (Highwind)    #
# ###################### #

# Variables

CC  := cc
OPT := -Wall -Wextra -Wno-switch -std=c99 -pedantic
LIB := -lSDL2
PRG := c_invaders
SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)

# Rules

all: debug

release: OPT += -O3
release: $(PRG)
	@rm -fv *.o

debug: OPT += -ggdb -O0 -DDEBUG
debug: $(PRG)

$(PRG): $(OBJ)
	$(CC) $^ -o $@ $(LIB)

src/main.o: src/main.c src/invaders.h
	$(CC) $(OPT) `sdl2-config --cflags` -c $< -o $@

src/invaders.o: src/invaders.c src/invaders.h
	$(CC) $(OPT) `sdl2-config --cflags` -c $< -o $@

src/graphics.o: src/graphics.c src/graphics.h
	$(CC) $(OPT) `sdl2-config --cflags` -c $< -o $@

src/s_title.o: src/s_title.c src/invaders.h
	$(CC) $(OPT) `sdl2-config --cflags` -c $< -o $@

src/s_play.o: src/s_play.c src/invaders.h
	$(CC) $(OPT) `sdl2-config --cflags` -c $< -o $@

assets/pack_sprites: assets/pack_sprites.c
	$(CC) $(OPT) `sdl2-config --cflags` $< -o $@ $(LIB)

# Clean

clean:
	@rm -fv $(OBJ)
	@rm -fv $(PRG)

.PHONY: all debug release clean


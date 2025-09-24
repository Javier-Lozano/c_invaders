# ###################### #
# Project: C Invaders    #
#  Author: Javier Lozano #
#          (Highwind)    #
# ###################### #

# Variables

CC  := cc
OPT := -Wall -Wextra -std=c99 -pedantic
LIB := -lSDL2
PRG := c_invaders
SRC := $(wildcard src/*.c)
OBJ := $(SRC:.c=.o)

# Rules

all: debug

release: OPT += -O3
release: $(PRG)
	@rm -fv $(OBJ)

debug: OPT += -ggdb -O0 -DDEBUG
debug: $(PRG)

$(PRG): $(OBJ)
	$(CC) $^ -o $@ $(LIB)

%.o: %.c
	$(CC) $(OPT) `sdl2-config --cflags` -c $< -o $@

# Clean

clean:
	@rm -fv $(OBJ)
	@rm -fv $(PRG)

.PHONY: all debug release clean


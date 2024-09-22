CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic -Iinc
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2 -lSDL2_image -lm

# List of source files and corresponding object files
SRC = src/main.c src/map_parser.c
OBJ = $(SRC:.c=.o)
EXEC = maze_game

# Default target to build the executable
all: $(EXEC)

# Rule to link object files into the final executable
$(EXEC): $(OBJ)
	$(CC) -o $@ $(OBJ) $(LDFLAGS)

# Rule to compile each source file into an object file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to check code style with Betty
betty:
	betty inc/*.h src/*.c

# Rule to clean up build artifacts
clean:
	rm -f $(OBJ) $(EXEC)

.PHONY: all clean
#include <stdio.h>
#include "map_parser.h"

int parse_map(const char *file_path) {
    FILE *file = fopen(file_path, "r");
    if (!file) {
        printf("Could not open file: %s\n", file_path);
        return 0; // Return 0 on failure
    }

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            char c = fgetc(file);
            if (c == '1') {
                maze[y][x] = 1; // Wall
            } else if (c == '0') {
                maze[y][x] = 0; // Empty space
            }
        }
        fgetc(file); // Read the newline character
    }

    fclose(file);
    return 1; // Return 1 on success
}

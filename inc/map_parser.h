#ifndef MAP_PARSER_H
#define MAP_PARSER_H

#include <stdio.h>
#include <stdlib.h>

/* Define the maze dimensions (ensure these match your maze files) */
#define MAZE_WIDTH 16
#define MAZE_HEIGHT 12

/* Declare the maze array */
extern int maze[MAZE_HEIGHT][MAZE_WIDTH];

/* Function prototype for parsing the map file */
int parse_map(const char *file_path, int maze[MAZE_HEIGHT][MAZE_WIDTH]);

#endif /* MAP_PARSER_H */

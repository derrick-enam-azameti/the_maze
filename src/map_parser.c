#include <stdio.h>
#include "map_parser.h"

/**
 * read_maze_file - Reads a maze from a file and populates the maze array.
 * @file: The file pointer to the maze file.
 * @maze: The array to store the parsed maze.
 *
 * Return: 1 on success, 0 on failure.
 */
static int read_maze_file(FILE *file, int maze[MAZE_HEIGHT][MAZE_WIDTH])
{
	for (int y = 0; y < MAZE_HEIGHT; y++)
	{
		for (int x = 0; x < MAZE_WIDTH; x++)
		{
			int c = fgetc(file);

			if (c == EOF)
			{
				printf("Unexpected end of file at (%d, %d)\n", y, x);
				return (0); /* Return 0 on failure */
			}
			if (c == '1')
			{
				maze[y][x] = 1; /* Wall */
			} else if (c == '0')
			{
				maze[y][x] = 0; /* Empty space */
			} else if (c == '\n')
			{
				x--; /* Ignore newline for column count */
			} else
			{
				printf("Invalid character '%c' at (%d, %d)\n", c, y, x);
				return (0); /* Return 0 on failure */
			}
		}
		fgetc(file); /* Read the newline character after each row */
	}
	return (1); /* Return 1 on success */
}


/**
 * parse_map - Parses a maze from a file.
 * @file_path: The path to the maze file.
 * @maze: The array to store the parsed maze.
 *
 * Return: 1 on success, 0 on failure.
 */
int parse_map(const char *file_path, int maze[MAZE_HEIGHT][MAZE_WIDTH])
{
	FILE *file = fopen(file_path, "r");

	if (!file)
	{
		printf("Could not open file: %s\n", file_path);
		return (0); /* Return 0 on failure */
	}

	int result = read_maze_file(file, maze);

	fclose(file);
	return (result); /* Return result from file reading */
}

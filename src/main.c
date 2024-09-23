#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "map_parser.h"
#include "raycast_maze.h"
#include "constants.h"
#include <stdbool.h>
#define COLLISION_BUFFER 0.1  /* Distance to maintain from walls */
#define MAP_FILE_PATH "./maps/maze1.map" /* Path to default maze */
#include "utils.c"
#include "player_movements.c"
#include "raycasting.c"

/**
 * init - Initializes the game environment.
 *
 * Description: Sets up window, loads textures, and initializes game variables.
 *
 * @game: Pointer to the Game structure that contains game state information.
 *
 * Return: 1 or 0 (success or failure)
 */
int init(Game *game)
{
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
		return (0);
	}
	/* Disable texture filtering */
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	game->window = SDL_CreateWindow(
		"3D Maze Game",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (game->window == NULL)
	{
		printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		return (0);
	}

	game->renderer = SDL_CreateRenderer(
		game->window, -1, SDL_RENDERER_ACCELERATED
	);

	if (game->renderer == NULL)
	{
		printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
		return (0);
	}

	return (1);
}

/**
 * main - The main entry point of the program.
 *
 * Description: TSets up the game loop, processes events,
 * updates the game state, and renders the graphics.
 * @argc: The number of command line arguments.
 * @argv: An array of command line argument strings
 *
 * Return:  0 (Success)
 */
int main(int argc, char *argv[])
{
	Game game = {NULL, NULL, {1.5, 1.5, 0}, NULL};/* starting point and angle */
	const char *mapFilePath = (argc > 1) ? argv[1] : MAP_FILE_PATH; /* Load map */
	int maze[MAZE_HEIGHT][MAZE_WIDTH];

	if (!parse_map(mapFilePath, maze)) /* Load the map */
	{
		printf("Failed to load the map from %s\n", mapFilePath);
		return (-1);
	}
	if (!init(&game))
	{
		printf("Failed to initialize!\n");
		return (-1);
	}
	load_textures(game.renderer, &groundTexture); /* Load textures */

	int quit = 0;
	SDL_Event e;

	while (!quit)
	{
		const Uint8 *state = SDL_GetKeyboardState(NULL);

		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT ||
				(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
			{
				quit = 1; /* Exit when ESC is pressed or window is closed */
			}
		}
		move_player(&game.player, state, maze);
		cast_rays(&game, &game.player, groundTexture, maze);
		SDL_RenderPresent(game.renderer);
	}
	/* Clean up textures */
	SDL_DestroyTexture(groundTexture);
	cleanup(&game);
	return (0);
}

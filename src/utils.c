#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include "map_parser.h"
#include "raycast_maze.h"
#include "constants.h"
#include <stdbool.h>

/* Global variable for the ground texture */
SDL_Texture *groundTexture;

/**
 * load_textures - Loads textures.
 *
 * Description: Loads textures.
 *
 * @renderer: renderer
 * @groundTexture: renders the ground texture
 *
 * Return: Nothing
 */
void load_textures(SDL_Renderer *renderer, SDL_Texture **groundTexture)
{
	*groundTexture = IMG_LoadTexture(renderer, "./images/grass14.png");

	if (!(*groundTexture))
	{
		printf("Failed to load ground texture: %s\n", SDL_GetError());
	}
}

/**
 * render_ground - renders the ground.
 *
 * Description: renders the ground.
 *
 * @renderer: renderer
 * @groundTexture: renders the ground texture
 *
 * Return: Nothing
 */
void render_ground(SDL_Renderer *renderer, SDL_Texture *groundTexture)
{
	int textureWidth, textureHeight;

	SDL_QueryTexture(groundTexture, NULL, NULL, &textureWidth, &textureHeight);
	/* Calculate how many times texture has to be repeated across the screen */
	/* +1 ensures no gaps at the edge */
	int tilesX = SCREEN_WIDTH / textureWidth + 1;
	int tilesY = (SCREEN_HEIGHT / 2) / textureHeight + 1;

	/* Render the ground by tiling the texture */
	for (int y = 0; y < tilesY; y++)
	{
		for (int x = 0; x < tilesX; x++)
		{
			SDL_Rect destRect = {x * textureWidth,
			SCREEN_HEIGHT / 2 + y * textureHeight,
			textureWidth, textureHeight
			};

			SDL_RenderCopy(renderer, groundTexture, NULL, &destRect);
		}
	}
}


/**
 * cleanup - cleans up the memory on game exit
 *
 * Description: cleans up the memory on game exit
 *
 * @game: pointer to game
 *
 * Return: Nothing
 */
void cleanup(Game *game)
{
	close_SDL(game);
}

/**
 * is_wall - Check if a given position is a wall in the maze
 *
 * Description: Check if a given position is a wall in the maze
 *
 * @x: x-position of player
 * @y: y-position of player
 * @maze: array of the maze
 *
 * Return: true
 */
bool is_wall(float x, float y, int maze[12][16])
{
	int map_x = (int)x;
	int map_y = (int)y;

	if (map_x < 0 || map_x >= 16 || map_y < 0 || map_y >= 12)
	{
		return (true);  /* Out of bounds */
	}

	return (maze[map_y][map_x] == 1);
}


/**
 * close_SDL - Clean up SDL resources
 *
 * Description: Clean up SDL resources
 *
 * @game: pointer to game struct
 *
 * Return: Nothing
 */
void close_SDL(Game *game)
{
	if (game->renderer != NULL)
	{
		SDL_DestroyRenderer(game->renderer);
	}
	if (game->window != NULL)
	{
		SDL_DestroyWindow(game->window);
	}
	SDL_Quit();
}

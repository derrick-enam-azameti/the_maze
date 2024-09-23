#ifndef RAYCAST_MAZE_H
#define RAYCAST_MAZE_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

/* Screen dimensions */
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

/* Player settings */
#define PLAYER_SPEED 0.01
#define ROTATION_SPEED 0.01
#define FOV 60.0
#define NUM_RAYS SCREEN_WIDTH


/* Struct to represent a player in the maze */
/**
 * struct Player - The player's position and orientation in the maze.
 * @x: The player's x-coordinate.
 * @y: The player's y-coordinate.
 * @angle: The angle at which the player is facing (in radians).
 */
typedef struct Player
{
	float x;
	float y;
	float angle;
} Player;

/* Define the Game struct */
typedef struct Game
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Player player;
    SDL_Texture *groundTexture;
} Game;

/* Function prototypes */
int init(Game *game);
void move_player(Player *player, const Uint8 *state, int maze[12][16]);
void cast_rays(Game *game, Player *player, SDL_Texture *groundTexture, int maze[12][16]);
bool is_wall(float x, float y, int maze[12][16]);


/* Function declarations */
void cleanup(Game *game);
void close_SDL(Game *game);


#endif /* RAYCAST_MAZE_H */

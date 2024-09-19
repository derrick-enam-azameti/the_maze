#ifndef RAYCAST_MAZE_H
#define RAYCAST_MAZE_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

// Screen dimensions
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// Player settings
#define PLAYER_SPEED 0.01
#define ROTATION_SPEED 0.005
#define FOV 60.0
#define NUM_RAYS SCREEN_WIDTH

// Player structure
typedef struct {
    float x;
    float y;
    float angle;
} Player;

// Function prototypes
int init();
void close_SDL();
void move_player(Player *player, const Uint8 *state, int maze[12][16]);
void cast_rays(SDL_Renderer *renderer, Player *player, int maze[12][16]);
bool is_wall(float x, float y, int maze[12][16]);

#endif // RAYCAST_MAZE_H

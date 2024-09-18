#include <SDL2/SDL.h>
#include <stdio.h>
#include "raycasting.h"
#include "raycast_maze.h"
#include "constants.h"
#include <stdbool.h>

// Global variables
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
Player player = {1.5, 1.5, 0};  // Starting position and angle

int maze[12][16] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

// Initialize SDL, create window and renderer
int init() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow("Raycasting Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 0;
    }

    return 1;
}

// Check if a given position is a wall in the maze
bool is_wall(float x, float y, int maze[12][16]) {
    int map_x = (int)x;
    int map_y = (int)y;

    if (map_x < 0 || map_x >= 16 || map_y < 0 || map_y >= 12) {
        return true;  // Out of bounds
    }

    return maze[map_y][map_x] == 1;
}

// Clean up SDL resources
void close_SDL() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

// Move player based on keyboard input and handle collisions
void move_player(Player *player, const Uint8 *state, int maze[12][16]) {
    float new_x = player->x;
    float new_y = player->y;

    if (state[SDL_SCANCODE_UP]) {
        // Calculate new position based on forward movement
        new_x = player->x + cos(player->angle) * PLAYER_SPEED;
        new_y = player->y + sin(player->angle) * PLAYER_SPEED;

        // Check for collisions and adjust position if needed
        if (!is_wall(new_x, new_y, maze)) {
            player->x = new_x;
            player->y = new_y;
        } else {
            // Slide along wall
            float slide_angle = player->angle + M_PI / 2;
            new_x = player->x + cos(slide_angle) * PLAYER_SPEED;
            new_y = player->y + sin(slide_angle) * PLAYER_SPEED;

            if (!is_wall(new_x, new_y, maze)) {
                player->x = new_x;
                player->y = new_y;
            }
        }
    }

    if (state[SDL_SCANCODE_DOWN]) {
        // Calculate new position based on backward movement
        new_x = player->x - cos(player->angle) * PLAYER_SPEED;
        new_y = player->y - sin(player->angle) * PLAYER_SPEED;

        // Check for collisions and adjust position if needed
        if (!is_wall(new_x, new_y, maze)) {
            player->x = new_x;
            player->y = new_y;
        } else {
            // Slide along wall
            float slide_angle = player->angle + M_PI / 2;
            new_x = player->x - cos(slide_angle) * PLAYER_SPEED;
            new_y = player->y - sin(slide_angle) * PLAYER_SPEED;

            if (!is_wall(new_x, new_y, maze)) {
                player->x = new_x;
                player->y = new_y;
            }
        }
    }

    if (state[SDL_SCANCODE_LEFT]) {
        // Rotate left
        player->angle -= ROTATION_SPEED;
    }

    if (state[SDL_SCANCODE_RIGHT]) {
        // Rotate right
        player->angle += ROTATION_SPEED;
    }
}

// Cast rays and render 3D walls with different colors based on orientation
void cast_rays(SDL_Renderer *renderer, Player *player, int maze[12][16]) {
    for (int i = 0; i < NUM_RAYS; i++) {
        // Calculate the ray's angle
        float ray_angle = player->angle - (FOV / 2.0) * (M_PI / 180) + i * (FOV * (M_PI / 180)) / NUM_RAYS;

        // Cast the ray step by step
        float ray_x = player->x;
        float ray_y = player->y;
        float distance_to_wall = 0;
        int hit_vertical = 0;  // 0 = horizontal wall, 1 = vertical wall

        while (1) {
            distance_to_wall += 0.01;
            ray_x = player->x + cos(ray_angle) * distance_to_wall;
            ray_y = player->y + sin(ray_angle) * distance_to_wall;

            // Check if the ray hits a wall
            if (maze[(int)ray_y][(int)ray_x] == 1) {
                // Determine if the hit was vertical (East/West) or horizontal (North/South)
                float delta_x = fabs(ray_x - player->x);
                float delta_y = fabs(ray_y - player->y);

                if (delta_x > delta_y) {
                    hit_vertical = 1;  // The wall is vertical (East/West)
                } else {
                    hit_vertical = 0;  // The wall is horizontal (North/South)
                }
                break;
            }
        }

        // Set the color based on the wall orientation
        if (hit_vertical) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // Red for vertical walls (East/West)
        } else {
            SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // Green for horizontal walls (North/South)
        }

        // Draw the wall column
        int line_height = (int)(SCREEN_HEIGHT / distance_to_wall);
        SDL_RenderDrawLine(renderer, i, (SCREEN_HEIGHT - line_height) / 2, i, (SCREEN_HEIGHT + line_height) / 2);
    }
}

int main() {
    if (!init()) {
        printf("Failed to initialize!\n");
        return -1;
    }

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        const Uint8 *state = SDL_GetKeyboardState(NULL);
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        move_player(&player, state, maze);

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // Black background
        SDL_RenderClear(renderer);

        // Cast rays to render 3D walls
        cast_rays(renderer, &player, maze);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    close_SDL();
    return 0;
}

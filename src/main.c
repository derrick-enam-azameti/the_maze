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
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 0, 0, 1},
    {1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
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
    // Set the color for the sky
    SDL_SetRenderDrawColor(renderer, 65, 149, 255, 255);  // Blue for sky
    SDL_RenderClear(renderer);  // Clear the screen to this color (sky)

    // Set the color for the ground
    SDL_SetRenderDrawColor(renderer, 23, 112, 0, 255);  // Green for ground
    SDL_Rect ground_rect = {0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2};
    SDL_RenderFillRect(renderer, &ground_rect);  // Draw the ground

    // Cast rays to draw walls
    for (int i = 0; i < NUM_RAYS; i++) {
        // Calculate the ray's angle
        float ray_angle = player->angle - (FOV / 2.0) * (M_PI / 180) + i * (FOV * (M_PI / 180)) / NUM_RAYS;

        // Step sizes
        float ray_dir_x = cos(ray_angle);
        float ray_dir_y = sin(ray_angle);

        // Position in grid space
        int map_x = (int)player->x;
        int map_y = (int)player->y;

        // Distance to the next x and y grid lines
        float delta_dist_x = fabs(1 / ray_dir_x);
        float delta_dist_y = fabs(1 / ray_dir_y);

        float side_dist_x, side_dist_y;

        // Step direction (-1 or 1) and initial distance to the first grid boundary
        int step_x, step_y;

        if (ray_dir_x < 0) {
            step_x = -1;
            side_dist_x = (player->x - map_x) * delta_dist_x;
        } else {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - player->x) * delta_dist_x;
        }

        if (ray_dir_y < 0) {
            step_y = -1;
            side_dist_y = (player->y - map_y) * delta_dist_y;
        } else {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - player->y) * delta_dist_y;
        }

        // Perform DDA to find the first wall hit
        int hit = 0;
        int side;  // 0 for horizontal, 1 for vertical
        while (hit == 0) {
            // Jump to next square
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 1;  // Vertical wall
            } else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 0;  // Horizontal wall
            }

            // Check if ray hits a wall
            if (maze[map_y][map_x] == 1) {
                hit = 1;
            }
        }

        // Calculate distance to the hit
        float perp_wall_dist;
        if (side == 0) {
            perp_wall_dist = (map_y - player->y + (1 - step_y) / 2) / ray_dir_y;
        } else {
            perp_wall_dist = (map_x - player->x + (1 - step_x) / 2) / ray_dir_x;
        }

        // Set the color based on the wall orientation
        if (side == 1) {
            SDL_SetRenderDrawColor(renderer, 93, 93, 93, 255);  // Light Gray for vertical walls (East/West)
        } else {
            SDL_SetRenderDrawColor(renderer, 77, 77, 77, 255);  // Dark Gray for horizontal walls (North/South)
        }

        // Draw the wall column
        int line_height = (int)(SCREEN_HEIGHT / perp_wall_dist);
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
        SDL_SetRenderDrawColor(renderer, 65, 149, 255, 255);  // Blue background
        SDL_RenderClear(renderer);

        // Cast rays to render 3D walls
        cast_rays(renderer, &player, maze);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    close_SDL();
    return 0;
}

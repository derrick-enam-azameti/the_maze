#include <math.h>
#include "raycasting.h"
#include "raycast_maze.h"
#include "constants.h"

// Define the map
int map[MAP_WIDTH][MAP_HEIGHT] = {
	{1, 1, 1, 1, 1, 1, 1, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 0, 1, 0, 1, 0, 0, 1},
	{1, 0, 1, 0, 1, 1, 0, 1},
	{1, 0, 0, 0, 0, 1, 0, 1},
	{1, 0, 1, 1, 0, 1, 0, 1},
	{1, 0, 0, 0, 0, 0, 0, 1},
	{1, 1, 1, 1, 1, 1, 1, 1}
};

void castRays(SDL_Renderer *renderer, double playerX, double playerY, double playerAngle)
{
	for (int i = 0; i < NUM_RAYS; i++)
	{
		double rayAngle = (playerAngle - FOV / 2.0) + ((double)i / NUM_RAYS) * FOV;

		double rayX = cos(rayAngle * M_PI / 180.0);
		double rayY = sin(rayAngle * M_PI / 180.0);

		double distToWall = 0;
		int hitWall = 0;

		while (!hitWall && distToWall < 10)
		{
			distToWall += 0.1;

			int mapX = (int)(playerX + rayX * distToWall);
			int mapY = (int)(playerY + rayY * distToWall);

			if (mapX >= 0 && mapX < MAP_WIDTH && mapY >= 0 && mapY < MAP_HEIGHT)
			{
				if (map[mapX][mapY] == 1)
				{
					hitWall = 1;
				}
			}
		}

		int lineHeight = (int)(WINDOW_HEIGHT / distToWall);
		int wallColor;

		// Determine wall orientation and set color
		if (fabs(rayX) > fabs(rayY))  // More horizontal direction
		{
			if (rayX > 0)
			{
				wallColor = 140;      // White for East-facing walls
			}
			else
			{
				wallColor = 27;      // Gray for West-facing walls
			}
		}
		else  // More vertical direction
		{
			if (rayY > 0)
			{
				wallColor = 88;        // Black for South-facing walls
			}
			else
			{
				wallColor = 92;      // White for North-facing walls
			}
		}

		// Draw the ceiling
        SDL_SetRenderDrawColor(renderer, 128, 0, 128, 255);  // Purple for the ceiling
        SDL_RenderDrawLine(renderer, i, 0, i, (WINDOW_HEIGHT - lineHeight) / 2);

        // Draw the ground
        SDL_SetRenderDrawColor(renderer, 10, 132, 0, 255);  // Yellow for the ground
        SDL_RenderDrawLine(renderer, i, (WINDOW_HEIGHT + lineHeight) / 2, i, WINDOW_HEIGHT);

        // Draw the wall
        SDL_SetRenderDrawColor(renderer, wallColor, wallColor, wallColor, 255);
        SDL_RenderDrawLine(renderer, i, (WINDOW_HEIGHT - lineHeight) / 2, i, (WINDOW_HEIGHT + lineHeight) / 2);
	}
}

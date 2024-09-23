/* Cast rays and render 3D walls with different colors based on orientation */
void cast_rays(Game *game, Player *player,
				SDL_Texture *groundTexture,
				int maze[12][16])
{
	/* Set the sky to seablue */
	SDL_SetRenderDrawColor(game->renderer, 65, 149, 255, 255);
	SDL_RenderClear(game->renderer);

	load_textures(game->renderer, &groundTexture);

	/* Render the ground with the texture */
	render_ground(game->renderer, groundTexture);

	/* Cast rays to draw walls */
	for (int i = 0; i < NUM_RAYS; i++)
	{
		/* Calculate the ray's angle */
		float ray_angle = player->angle - (FOV / 2.0) * (M_PI / 180) +
							i * (FOV * (M_PI / 180)) / NUM_RAYS;

		/* Step sizes */
		float ray_dir_x = cos(ray_angle);
		float ray_dir_y = sin(ray_angle);

		/* Position in grid space */
		int map_x = (int)player->x;
		int map_y = (int)player->y;

		/* Distance to the next x and y grid lines */
		float delta_dist_x = fabs(1 / ray_dir_x);
		float delta_dist_y = fabs(1 / ray_dir_y);

		float side_dist_x, side_dist_y;

		/* Step direction and initial distance to the first grid boundary */
		int step_x, step_y;

		if (ray_dir_x < 0)
		{
			step_x = -1;
			side_dist_x = (player->x - map_x) * delta_dist_x;
		} else
		{
			step_x = 1;
			side_dist_x = (map_x + 1.0 - player->x) * delta_dist_x;
		}

		if (ray_dir_y < 0)
		{
			step_y = -1;
			side_dist_y = (player->y - map_y) * delta_dist_y;
		} else
		{
			step_y = 1;
			side_dist_y = (map_y + 1.0 - player->y) * delta_dist_y;
		}

		/* Perform DDA to find the first wall hit */
		int hit = 0;
		int side;  /* 0 for horizontal, 1 for vertical */

		while (hit == 0)
		{
			/* Jump to next square */
			if (side_dist_x < side_dist_y)
			{
				side_dist_x += delta_dist_x;
				map_x += step_x;
				side = 1;  /* Vertical wall */
			} else
			{
				side_dist_y += delta_dist_y;
				map_y += step_y;
				side = 0;  /* Horizontal wall */
			}

			/* Check if ray hits a wall */
			if (maze[map_y][map_x] == 1)
			{
				hit = 1;
			}
		}

		/* Calculate distance to the hit */
		float perp_wall_dist;

		if (side == 0)
		{
			perp_wall_dist = (map_y - player->y + (1 - step_y) / 2) / ray_dir_y;
		} else
		{
			perp_wall_dist = (map_x - player->x + (1 - step_x) / 2) / ray_dir_x;
		}

		/* Set the color based on the wall orientation */
		if (side == 1)
		{
			/* Light Gray for vertical walls (East/West) */
			SDL_SetRenderDrawColor(game->renderer, 93, 93, 93, 255);
		} else
		{
			/* Dark Gray for horizontal walls (North/South) */
			SDL_SetRenderDrawColor(game->renderer, 77, 77, 77, 255);
		}

		/* Draw the wall column */
		int line_height = (int)(SCREEN_HEIGHT / perp_wall_dist);

		SDL_RenderDrawLine(game->renderer, i, (SCREEN_HEIGHT - line_height) / 2,
				i, (SCREEN_HEIGHT + line_height) / 2);
	}
}
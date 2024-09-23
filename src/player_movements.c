/**
 * move_player - Move player based on keyboard input and handle collisions
 *
 * Description: Move player based on keyboard input and handle collisions
 *
 * @player: player struct pointer
 * @state: pointer
 * @maze: array of the maze
 *
 * Return: Nothing
 */
void move_player(Player *player, const Uint8 *state, int maze[12][16])
{
	float new_x = player->x;
	float new_y = player->y;

	if (state[SDL_SCANCODE_UP])
	{
		/* Calculate new position based on forward movement */
		new_x = player->x + cos(player->angle) * PLAYER_SPEED;
		new_y = player->y + sin(player->angle) * PLAYER_SPEED;

		/* Check for collisions and adjust position if needed */
		if (!is_wall(new_x, new_y, maze))
		{
			player->x = new_x;
			player->y = new_y;
		} else
		{
			float slide_angle = player->angle + M_PI / 2;

			new_x = player->x + cos(slide_angle) * (PLAYER_SPEED + COLLISION_BUFFER);
			new_y = player->y + sin(slide_angle) * (PLAYER_SPEED + COLLISION_BUFFER);
			if (!is_wall(new_x, new_y, maze))
			{
				player->x = new_x;
				player->y = new_y;
			}
		}
	}

	if (state[SDL_SCANCODE_DOWN])
	{
		new_x = player->x - cos(player->angle) * PLAYER_SPEED;
		new_y = player->y - sin(player->angle) * PLAYER_SPEED;

		if (!is_wall(new_x, new_y, maze))
		{
			player->x = new_x;
			player->y = new_y;
		} else
		{
			float slide_angle = player->angle + M_PI / 2;

			new_x = player->x - cos(slide_angle) * PLAYER_SPEED;
			new_y = player->y - sin(slide_angle) * PLAYER_SPEED;

			if (!is_wall(new_x, new_y, maze))
			{
				player->x = new_x;
				player->y = new_y;
			}
		}
	}

	if (state[SDL_SCANCODE_LEFT])
	{
		player->angle -= ROTATION_SPEED;
	}

	if (state[SDL_SCANCODE_RIGHT])
	{
		player->angle += ROTATION_SPEED;
	}
}


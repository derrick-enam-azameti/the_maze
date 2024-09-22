# Raycasting Maze Game

## Description

This project is a simple 3D maze game built using **C** and **SDL2**, with raycasting to render the 3D environment. The player can navigate through a maze, and walls are drawn with distinct colors based on their orientation. The ceiling and ground are also color-coded for clarity.

## Features

- 3D rendering using raycasting.
- Player movement with collision detection.
- Color-coded walls based on direction:
	- **Dark Gray** for vertical (East/West) walls.
	- **Gray** for horizontal (North/South) walls.
- Ground and ceiling color-coded for visual distinction:
	- **Green Textured Graphic** ground.
	- **Seablue** ceiling.
- Simple 2D grid-based maze.
- Option to load different maze layouts from a file.

## How to Play

- Use arrow keys to move:
	- **Up arrow** to move forward.
	- **Down arrow** to move backward.
	- **Left/Right arrows** to rotate the view.

Avoid walls and navigate through the maze to explore the environment.

## Installation and Compilation

### Prerequisites

- **SDL2** library: Install SDL2 core library on your system if you haven't already.
- **SDL2 Image** library: Install SDL2_image library for handling image files.

### Build Instructions

1. Clone the repository:
	 ```bash
	 git clone https://github.com/derrick-enam-azameti/the_maze.git
	 cd the_maze

2. Compile the project:
	 ```bash
	 make

3. Run the game with the default maze layout:
	 ```bash
	 ./maze_game

4. Run the game with a specific maze layout (e.g. maze2.map)
	 ```bash
	 ./maze_game maps/maze2.map

## Contribution

Feel free to fork the repository and submit pull requests for any enhancements or bug fixes.

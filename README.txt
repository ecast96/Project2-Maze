CSCI 115 - Project 2
Work done by Erick Castro and Nina Lee

Project contains a game constructed from Dr. Dhanyu Amarasinghe's MAZE Game Framework.

Graphics used were created by Daniel Eddeland (https://opengameart.org/content/lpc-style-farm-animals)

Game was created using CodeBLock 13.12.
 - Given framework uses OpenGL and SOIL libraries for displaying


Game Information: 

The game is a simple turned based game with a toggle switch for shooting in various duration.
 - You can move player with arrow keys (left/right/up/down) by one tile at a time.
 - To shoot, you must press the key 'z' to toggle between moving and shooting. 
   - You can shooting in any direction (left/right/up/down) using the arrow keys.

If the player reaches the chest, you win.
If the player collides with an enemy, you lose.


How it works:

A level text file contains the coordinates of where objcts(walls, enemies,...) will be placed in matrix.
 - The readFile() function will read that text file and places objcts into the matrix.

The values for the objects are:
- 1 = wall
- 2 = enemy
- 3 = player
- 4 = chest
- 5 = set of arrows

As player and enemies move, the matrix is updated to reflect that.

How to run:

1. Open the CodeBlocks project file named "Maze.cbp".
   - You will need to add all .cpp and .h files to project using "Project/Add files" EXCEPT CommonThings.cpp.
2. Compile and run game.

Files included are:
 - commonthings.h (framework)
 - enemies.cpp/.h (framework)
 - graph.cpp/.h
 - maze.cpp/.h (framework)
 - player.cpp/.h (framework)
 - timer.cpp/.h (framework)
 - wall.cpp/.h (framework)
 - main.cpp/.h (framework) // where most of code was implemented
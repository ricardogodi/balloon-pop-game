# Balloon-Pop Game Engine

## Overview
This project is a C implementation of a dynamic and interactive Balloon-Pop game, inspired by similar puzzle games. It features advanced memory management, recursive algorithms for gameplay mechanics, and a robust game state management system using Abstract Data Types (ADTs).

## Technical Description
This engine implements an Abstract Data Type (ADT) to manage game states including the board layout, score tracking, and undo functionality. It uses dynamic memory allocation for storing game states and utilizes recursive algorithms for balloon popping based on color clusters. The game logic includes detecting clusters of balloons, popping them based on user input, and implementing ‘gravity’ to make balloons rise, mimicking helium effects. This project emphasizes the use of pointers, structures, and memory management to create an engaging and responsive game experience while maintaining code modularity and encapsulation.

## Files Included

### bpgame.c
- **Description:** Implements the game logic for Balloon Pop, including board initialization, balloon popping mechanics, and score management.
- **Author:** Ricardo Gonzalez

### bpgame.h
- **Description:** Header file for the Balloon Pop game engine. Defines the `BPGame` data structure and prototypes for game management functions.
- **Author:** Ricardo Gonzalez

### bpop.c
- **Description:** Provides a terminal-based user interface for the Balloon Pop game, handling user input and displaying game status.
- **Author:** Ricardo Gonzalez

### simpleio.c
- **Description:** Contains simplified input/output functions adapted from the CS50 Library to facilitate user interaction in terminal.
- **Copyright:** (c) 2013, Glenn Holloway <holloway@eecs.harvard.edu>, David J. Malan <malan@harvard.edu>. All rights reserved. Licensed under the BSD 3-Clause License.

### simpleio.h
- **Description:** Header file for the simple I/O library functions, providing prototypes for functions used in `simpleio.c`.
- **Copyright:** (c) 2013, Glenn Holloway <holloway@eecs.harvard.edu>, David J. Malan <malan@harvard.edu>. All rights reserved. Licensed under the BSD 3-Clause License.

## Instructions for Running the Game

### Compilation
Run the following command in the project directory to compile the game:
```bash
make all
```

### Execution
To start the game, execute:
```bash
./bpop
```

## Gameplay

- Enter coordinates (row and column) to "pop" balloons. Balloons will pop if they form a cluster of at least two balloons of the same color.
- Type `u` to undo the last move.
- Type `q` to quit the game.
- The game tracks and displays the score based on the number of balloons popped.
- The game ends when no more pops are possible or the user quits.


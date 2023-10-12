# COMP0002 Principles of Programming, C Coursework <br>Maze Auto-generator and Auto-solver

## About

Maze Auto-generator and Auto-solver first generates a solvable maze, which has the size of 15 × 15, using randomized Kruskal's algorithm.
Then, the robot will search the way from the starting point to the exit marker, using Depth-first search (DPS).
The program shows the robot's motion as it searches the exit from the start position.

## Command
To run this program, the following files should be in the same directory.

drawapp-2.0.jar\
graphics.h\
graphics.c\
gcc -o maze maze.c graphics.c\
./maze | java -jar drawapp-2.0.jar

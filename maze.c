#include <stdio.h>
#include "graphics.h"
#include <stdlib.h>
#include <time.h>

// These are the constants that will be used in this program
enum {
    WINDOW_HEIGHT = 1500,
    WINDOW_WIDTH = 1500,
    MAZE_HEIGHT = 17,
    MAZE_WIDTH = 17,
    MAZE_CORNER = 30,
    SQUARE_SIDE = 40,
    ROBOT_HEIGHT = 40,
};

void initMaze(int maze[MAZE_HEIGHT][MAZE_WIDTH])
{
    // Create floor base (floor means 1)
    for (int row = 0; row < MAZE_HEIGHT; row++){
        for (int column = 0; column < MAZE_WIDTH; column++) maze[row][column] = 1; 
    }
    
    // Create walls around the base (wall means 0)
    for (int row = 0; row < MAZE_HEIGHT; row += 1){
        maze[row][0] = 0;
        maze[row][MAZE_WIDTH - 1] = 0;
    }
    for (int column = 0; column < MAZE_WIDTH; column += 1){
        maze[0][column] = 0;
        maze[MAZE_HEIGHT - 1][column] = 0;
    }
}

// This recursive function uses randomized Kruskal's algorithm to generate a maze
void createMaze(int maze[MAZE_HEIGHT][MAZE_WIDTH])
{
    for (int row = 2; row < MAZE_HEIGHT - 1; row += 2){
        for (int column = 2; column < MAZE_WIDTH - 1; column += 2){
            // Create a wall at the square in the maze on the even row and column if the square is currently a floor
            if (maze[row][column] == 1){
                maze[row][column] = 0;
                // Randomly choose a direction to extend the wall by 2 rows or columns
                int direction = rand() % 4;
                switch (direction) {
                    // North
                    case 0:
                        maze[row + 1][column] = 0;
                         // If the next next square is already a wall, break the loop and move to the next iteration
                        if (maze[row + 2][column] == 0) break; 
                        else maze[row + 2][column] = 0;
                    // South
                    case 1:
                        maze[row - 1][column] = 0;
                        if (maze[row - 2][column] == 0) break;
                        else maze[row - 2][column] = 0;
                    // East
                    case 2:
                        maze[row][column + 1] = 0;
                        if (maze[row][column + 2] == 0) break;
                        else maze[row][column + 2] = 0;
                    // West
                    case 3:
                        maze[row][column - 1] = 0;
                        if (maze[row][column - 2] == 0) break;
                        else maze[row][column - 2] = 0;
                }
                // Recursively call the createMaze function to create walls until the condition at the top will not be met
                createMaze(maze);
            }
        }
    }
}

void drawSquares(int maze[MAZE_HEIGHT][MAZE_WIDTH])
{
    for (int x_coordinate = MAZE_CORNER; x_coordinate <= MAZE_CORNER + (MAZE_WIDTH - 1) * SQUARE_SIDE; x_coordinate += SQUARE_SIDE) {
        for (int y_coordinate = MAZE_CORNER; y_coordinate <= MAZE_CORNER + (MAZE_HEIGHT - 1) * SQUARE_SIDE; y_coordinate += SQUARE_SIDE) {
            // Draw squares on the canvas
            drawRect(x_coordinate, y_coordinate, SQUARE_SIDE, SQUARE_SIDE);
            int column = (x_coordinate - MAZE_CORNER) / SQUARE_SIDE;
            int row = (y_coordinate - MAZE_CORNER) / SQUARE_SIDE;
            // Fill the square with black if it is a wall
            if (maze[row][column] == 0) setColour(black);
            // Fill the square with white if it is a floor
            else setColour(white);
            fillRect(x_coordinate, y_coordinate, SQUARE_SIDE, SQUARE_SIDE);
        }
    }
}

void setStart(int start[2])
{
    // Set start position at random from floors
    int start_x = rand() % (MAZE_WIDTH - 2);
    int start_y = rand() % (MAZE_HEIGHT - 2);
    start_x += start_x % 2 == 0 ? 1: 0;
    start_y += start_y % 2 == 0 ? 1: 0;
    // Store start position into start[2]
    start[0] = start_y;
    start[1] = start_x;
}

void initRobot(int *robotPosition_x, int *robotPosition_y, int maze[MAZE_HEIGHT][MAZE_WIDTH], int robotPath[MAZE_HEIGHT][MAZE_WIDTH], int currentPosition[2], int start[2])
{
    // Copy maze information into robotPath 2D array, which will be used to trace the path of the robot
    for (int row = 0; row < MAZE_HEIGHT; row++){
        for (int column = 0; column < MAZE_WIDTH; column++) robotPath[row][column] = maze[row][column]; 
    }
    setStart(start);
    // Initialize currenPosition by inserting start position
    currentPosition[0] = start[0];
    currentPosition[1] = start[1];
    // Initialize robotPosition_x and robotPosition_y by calculating each coordinate based on the start position
    robotPosition_x[0] = MAZE_CORNER + (start[1]) * SQUARE_SIDE;
    robotPosition_x[1] = MAZE_CORNER + (start[1]) * SQUARE_SIDE + ROBOT_HEIGHT;
    robotPosition_x[2] = MAZE_CORNER + (start[1]) * SQUARE_SIDE;
    for (int i = 0; i <= 2; i++) robotPosition_y[i] = MAZE_CORNER + (start[0]) * SQUARE_SIDE + i * ROBOT_HEIGHT / 2;
    
}

void setGoal(int start[2], int goal[2])
{
    int goal_x, goal_y;
    // Set goal position at random from floors
    goal_x = rand() % (MAZE_HEIGHT - 2);
    goal_y = rand() % (MAZE_WIDTH - 2);
    goal_x += goal_x % 2 == 0 ? 1: 0;
    goal_y += goal_y % 2 == 0 ? 1: 0;
    // Repeat if the start and the goal are the same
    if (start[1] == goal_x && start[0] == goal_y) setGoal(start, goal);
    // Store goal position into goal[2]
    else{
        goal[0] = goal_y;
        goal[1] = goal_x;
    }
}

void drawGoal(int start[2], int goal[2])
{
    // Draw the goal by filling a square with gray
    setGoal(start, goal);
    setColour(gray);
    drawRect(MAZE_CORNER + goal[1] * SQUARE_SIDE, MAZE_CORNER + goal[0] * SQUARE_SIDE, SQUARE_SIDE, SQUARE_SIDE);
    fillRect(MAZE_CORNER + goal[1] * SQUARE_SIDE, MAZE_CORNER + goal[0] * SQUARE_SIDE, SQUARE_SIDE, SQUARE_SIDE);
}

void drawMaze(int maze[MAZE_HEIGHT][MAZE_WIDTH], int start[2], int goal[2])
{
    setWindowSize(WINDOW_HEIGHT, WINDOW_WIDTH);
    background();
    initMaze(maze);
    createMaze(maze);
    drawSquares(maze);
    drawGoal(start, goal);
    // Draw lines around all the squares
    for (int x_coordinate = MAZE_CORNER; x_coordinate <= MAZE_CORNER + (MAZE_WIDTH - 1) * SQUARE_SIDE; x_coordinate += SQUARE_SIDE){
        setColour(black);
        drawLine(x_coordinate, MAZE_CORNER, x_coordinate, MAZE_CORNER + (MAZE_HEIGHT - 1) * SQUARE_SIDE);
        drawLine(MAZE_CORNER, x_coordinate, MAZE_CORNER + (MAZE_HEIGHT - 1) * SQUARE_SIDE, x_coordinate);
    }
}

void drawRobot(int *robotPosition_x, int *robotPosition_y)
{
    // Draw a robot every 500ms based on their coordinates
    foreground();
    setColour(green);
    drawPolygon(3, robotPosition_x, robotPosition_y);
    fillPolygon(3, robotPosition_x, robotPosition_y);
    sleep(500);
    clear();
}

char detectDirection(int *robotPosition_x, int *robotPosition_y)
{
    // Detect the robot's direction based on their coordinates
    char direction;
    if (robotPosition_y[0] == robotPosition_y[2]) direction = robotPosition_y[1] > robotPosition_y[0] ? 'S' : 'N';
    else direction = robotPosition_x[1] > robotPosition_x[0] ? 'E' : 'W';
    return direction;
}

// This function determines whether it is possible to move forward or not
int canMoveForward(int *robotPosition_x, int *robotPosition_y, int robotPath[MAZE_HEIGHT][MAZE_WIDTH], int currentPosition[2])
{
    char direction = detectDirection(robotPosition_x, robotPosition_y);
    int row = currentPosition[0];
    int column = currentPosition[1];
    // Get the row and column of the next square that the robot is trying to enter based on the direction
    if (direction == 'N') row = row - 1;
    else if (direction == 'S') row = row + 1;
    else if (direction == 'E') column = column + 1;
    else if (direction == 'W') column = column - 1;
    // Return 1 if the next column is floor and return 0 if the next column is a wall
    if (robotPath[row][column] == 1) return 1;
    else if (robotPath[row][column] == 0) return 0;   
}

void forward(int *robotPosition_x, int *robotPosition_y, int currentPosition[2])
{
    char direction = detectDirection(robotPosition_x, robotPosition_y);
    // Calculate the coordinates of the robot after moving forward based on the direction
    if (direction == 'N') {
        for (int i = 0; i <= 2; ++i) robotPosition_y[i] = robotPosition_y[i] - SQUARE_SIDE;
        currentPosition[0] = currentPosition[0] - 1;
    }
    else if (direction == 'S') {
        for (int i = 0; i <= 2; ++i) robotPosition_y[i] = robotPosition_y[i] + SQUARE_SIDE;
        currentPosition[0] = currentPosition[0] + 1;
    }
    else if (direction == 'E') {
        for (int i = 0; i <= 2; ++i) robotPosition_x[i] = robotPosition_x[i] + SQUARE_SIDE;
        currentPosition[1] = currentPosition[1] + 1;
    }
    else if (direction == 'W') {
        for (int i = 0; i <= 2; ++i) robotPosition_x[i] = robotPosition_x[i] - SQUARE_SIDE;
        currentPosition[1] = currentPosition[1] - 1;
    }
    drawRobot(robotPosition_x, robotPosition_y);
}

void left(int *robotPosition_x, int *robotPosition_y)
{
    // For a robot pointing North or South
    if (robotPosition_y[0] == robotPosition_y[2]){
        // Calculate coordinates which will change by turning left
        robotPosition_x[0] = robotPosition_y[1] > robotPosition_y[0] ? robotPosition_x[0] - ROBOT_HEIGHT : robotPosition_x[0] + ROBOT_HEIGHT;
        robotPosition_x[1] = robotPosition_y[1] > robotPosition_y[0] ? robotPosition_x[1] + 20 : robotPosition_x[1] - 20;
        robotPosition_y[1] = robotPosition_y[1] > robotPosition_y[0] ? robotPosition_y[1] - 20 : robotPosition_y[1] + 20;
        robotPosition_y[2] = robotPosition_y[1] > robotPosition_y[0] ? robotPosition_y[2] + ROBOT_HEIGHT : robotPosition_y[2] - ROBOT_HEIGHT;
    }
    // For a robot pointing East or West
    else {
        // Calculate coordinates which will change by turning left
        robotPosition_y[0] = robotPosition_y[1] < robotPosition_y[0] ? robotPosition_y[0] - ROBOT_HEIGHT : robotPosition_y[0] + ROBOT_HEIGHT;
        robotPosition_x[1] = robotPosition_y[1] < robotPosition_y[0] ? robotPosition_x[1] - ROBOT_HEIGHT / 2 : robotPosition_x[1] + ROBOT_HEIGHT / 2;
        robotPosition_y[1] = robotPosition_y[1] < robotPosition_y[0] ? robotPosition_y[1] - ROBOT_HEIGHT / 2 : robotPosition_y[1] + ROBOT_HEIGHT / 2;
        robotPosition_x[2] = robotPosition_y[1] < robotPosition_y[0] ? robotPosition_x[2] + ROBOT_HEIGHT : robotPosition_x[2] - ROBOT_HEIGHT;
    }
    drawRobot(robotPosition_x, robotPosition_y);
}

void right(int *robotPosition_x, int *robotPosition_y)
{
    // For a robot pointing North or South
    if (robotPosition_y[0] == robotPosition_y[2]){
        // Calculate coordinates which will change by turning right
        robotPosition_y[0] = robotPosition_x[1] > robotPosition_x[0] ? robotPosition_y[0] - ROBOT_HEIGHT : robotPosition_y[0] + ROBOT_HEIGHT;
        robotPosition_x[1] = robotPosition_x[1] > robotPosition_x[0] ? robotPosition_x[1] + 20 : robotPosition_x[1] - 20;
        robotPosition_y[1] = robotPosition_x[1] > robotPosition_x[0] ? robotPosition_y[1] + 20 : robotPosition_y[1] - 20;
        robotPosition_x[2] = robotPosition_x[1] > robotPosition_x[0] ? robotPosition_x[2] - ROBOT_HEIGHT : robotPosition_x[2] + ROBOT_HEIGHT;
    }
    // For a robot pointing East or West
    else {
        // Calculate coordinates which will change by turning right
        robotPosition_x[0] = robotPosition_y[1] < robotPosition_y[0] ? robotPosition_x[0] - ROBOT_HEIGHT : robotPosition_x[0] + ROBOT_HEIGHT;
        robotPosition_x[1] = robotPosition_y[1] < robotPosition_y[0] ? robotPosition_x[1] + ROBOT_HEIGHT / 2 : robotPosition_x[1] - ROBOT_HEIGHT / 2;
        robotPosition_y[1] = robotPosition_y[1] < robotPosition_y[0] ? robotPosition_y[1] - ROBOT_HEIGHT / 2 : robotPosition_y[1] + ROBOT_HEIGHT / 2;
        robotPosition_y[2] = robotPosition_y[1] < robotPosition_y[0] ? robotPosition_y[2] + ROBOT_HEIGHT : robotPosition_y[2] - ROBOT_HEIGHT;
    }
    drawRobot(robotPosition_x, robotPosition_y);
}

int atMarker(int currentPosition[2], int goal[2])
{
    // Return 1 if the robot reaches the goal
    if (currentPosition[0] == goal[0] && currentPosition[1] == goal[1]) {
        printf("goal!!!");
        return 1;
    }
    else return 0;
}

// The following four functions (goNorth, goSouth, goEast, goWest) move the robot to the intended direction based on the current direction
void goNorth(int *robotPosition_x, int *robotPosition_y, int currentPosition[2])
{
    char direction = detectDirection(robotPosition_x, robotPosition_y);
    if (direction == 'S') {
        right(robotPosition_x, robotPosition_y);
        right(robotPosition_x, robotPosition_y);
    }
    else if (direction == 'E') left(robotPosition_x, robotPosition_y);
    else if (direction == 'W') right(robotPosition_x, robotPosition_y);
    forward(robotPosition_x, robotPosition_y, currentPosition);
}

void goSouth(int *robotPosition_x, int *robotPosition_y, int currentPosition[2])
{
    char direction = detectDirection(robotPosition_x, robotPosition_y);
    if (direction == 'N') {
        right(robotPosition_x, robotPosition_y);
        right(robotPosition_x, robotPosition_y);
    }
    else if (direction == 'E') right(robotPosition_x, robotPosition_y);
    else if (direction == 'W') left(robotPosition_x, robotPosition_y);
    forward(robotPosition_x, robotPosition_y, currentPosition);
}

void goEast(int *robotPosition_x, int *robotPosition_y, int currentPosition[2])
{
    char direction = detectDirection(robotPosition_x, robotPosition_y);
    if (direction == 'W') {
        right(robotPosition_x, robotPosition_y);
        right(robotPosition_x, robotPosition_y);
    }
    else if (direction == 'N') right(robotPosition_x, robotPosition_y);
    else if (direction == 'S') left(robotPosition_x, robotPosition_y);
    forward(robotPosition_x, robotPosition_y, currentPosition);
}

void goWest(int *robotPosition_x, int *robotPosition_y, int currentPosition[2])
{
    char direction = detectDirection(robotPosition_x, robotPosition_y);
    if (direction == 'E') {
        right(robotPosition_x, robotPosition_y);
        right(robotPosition_x, robotPosition_y);
    }
    else if (direction == 'N') left(robotPosition_x, robotPosition_y);
    else if (direction == 'S') right(robotPosition_x, robotPosition_y);
    forward(robotPosition_x, robotPosition_y, currentPosition);
}

// This function makes the robot move back along the path that robot has taken so far
void backTrack(int *robotPosition_x, int *robotPosition_y, int maze[MAZE_HEIGHT][MAZE_WIDTH], int currentPosition[2])
{
    int row = currentPosition[0];
    int column = currentPosition[1];
    char direction = detectDirection(robotPosition_x, robotPosition_y);
    // Find and move to a floor from the original maze array, which is not the one that the robot is positioned currently
    if (direction != 'E' && maze[row - 1][column] == 1) goNorth(robotPosition_x, robotPosition_y, currentPosition);
    else if (direction != 'W' && maze[row + 1][column] == 1) goSouth(robotPosition_x, robotPosition_y, currentPosition);
    else if (direction != 'N' && maze[row][column - 1] == 1) goWest(robotPosition_x, robotPosition_y, currentPosition);
    else if (direction != 'S' && maze[row][column + 1] == 1) goEast(robotPosition_x, robotPosition_y, currentPosition);
}

// This recursive function used Depth-first search (DFS) to search the path from the start to the goal
void move(int *robotPosition_x, int *robotPosition_y, int maze[MAZE_HEIGHT][MAZE_WIDTH], int robotPath[MAZE_HEIGHT][MAZE_WIDTH], int currentPosition[2], int goal[2])
{
    // Return if the robot reaches the goal
    if (atMarker(currentPosition, goal) == 1) return;
    int row = currentPosition[0];
    int column = currentPosition[1];
    // Change "1" of the robotPath at the currentPosition to "0" which the robot cannot enter, to show that the robot has visited the square
    robotPath[row][column] = 0;
    // Move forward as far as the robot can 
    if (canMoveForward(robotPosition_x, robotPosition_y, robotPath, currentPosition) == 1) forward(robotPosition_x, robotPosition_y, currentPosition);
    else {
        // If the next square is a wall, turn right
        right(robotPosition_x, robotPosition_y);
        // If the next square is a floor, call move function again
        if (canMoveForward(robotPosition_x, robotPosition_y, robotPath, currentPosition) == 1);
        else{
            // If the next square is a wall, turn left twice to check the last neighbor square that robot has not checked
            left(robotPosition_x, robotPosition_y);
            left(robotPosition_x, robotPosition_y);
            // If the next square is a floor, call move function again
            if (canMoveForward(robotPosition_x, robotPosition_y, robotPath, currentPosition) == 1) forward(robotPosition_x, robotPosition_y, currentPosition);
            else {
                // If the next square is a wall, it means that the square has no way to reach the goal
                // Thus, Change the floor of the maze at the currentPosition to "0" which the robot cannot enter permanently
                maze[row][column] = 0;
                // Then, backtrack to the previous square and call move function again
                backTrack(robotPosition_x, robotPosition_y, maze, currentPosition);
            }
        }
    }
    move(robotPosition_x, robotPosition_y, maze, robotPath, currentPosition, goal);
}

int main(void)
{
    // Initialize all the arrays
    int robotPosition_x[3], robotPosition_y[3], currentPosition[2], robotPath[MAZE_HEIGHT][MAZE_WIDTH], maze[MAZE_HEIGHT][MAZE_WIDTH], start[2], goal[2];
    // Reset the seed of the random function, in order to generate a different maze every time
    srand(time(NULL));
    drawMaze(maze, start, goal);
    initRobot(robotPosition_x, robotPosition_y, maze, robotPath, currentPosition, start);
    drawRobot(robotPosition_x, robotPosition_y);
    move(robotPosition_x, robotPosition_y, maze, robotPath, currentPosition, goal);
}
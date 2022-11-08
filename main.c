#include "graphics.h"
#include <stdlib.h>

const int maze_size = 60;
int maze[maze_size][maze_size];
int stackX[maze_size * maze_size];
int stackY[maze_size * maze_size];
int stackX_count = 0;
int stackY_count = 0;
void pushX(int coord_x) {
  stackX[stackX_count] = coord_x;
  stackX_count++;
}

int popX() {
  int item = stackX[stackX_count - 1];
  stackX_count--;
  return item;
}

void pushY(int coord_y) {
  stackY[stackY_count] = coord_y;
  stackY_count++;
}

int popY() {
  int item = stackY[stackY_count - 1];
  stackY_count--;
  return item;
}


void setBaseMaze() {
  for (int col = 0; col < maze_size; col++) {
    for (int row = 0; row < maze_size; row++) {
      maze[row][col] = 0;
    }
  }
}

// counter for the number of cells visited by the robot
int visited = 0;
// array for storing the (x,y) of 'neighbours' of a given cell
int nbX[4];
int nbY[4];
int nb_count = 0;
// initial (x,y) coordinates of the robot's position
int posX = 0;
int posY = 0;
int visitedX[maze_size * maze_size];
int visitedY[maze_size * maze_size];
// check if valid neighbour exists in NESW directions
int nb_N(int x, int y) {
  int seen = 0;
  if ((y - 2) > -1) {
    for (int i = 0; i < visited; i++) {
      if (x == visitedX[i] && y - 2 == visitedY[i]) {
        seen = 1;
      }
    }
    if (seen == 0) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}

int nb_E(int x, int y) {
  int seen = 0;
  if ((x + 2) < maze_size) {
    for (int i = 0; i < visited; i++) {
      if (x + 2 == visitedX[i] && y == visitedY[i]) {
        seen = 1;
      }
    }
    if (seen == 0) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}

int nb_S(int x, int y) {
  int seen = 0;
  if ((y + 2) < maze_size) {
    for (int i = 0; i < visited; i++) {
      if (x == visitedX[i] && y + 2 == visitedY[i]) {
        seen = 1;
      }
    }
    if (seen == 0) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}

int nb_W(int x, int y) {
  int seen = 0;
  if ((x - 2) > -1) {
    for (int i = 0; i < visited; i++) { 
      if (x - 2 == visitedX[i] && y == visitedY[i]) {
        seen = 1;
      }
    }
    if (seen == 0) {
      return 1;
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}

void getNeighbors() {
  if (nb_N(posX, posY) == 1) {
    nbX[nb_count] = posX;
    nbY[nb_count] = posY - 2;
    nb_count++;
  }
  if (nb_E(posX, posY) == 1) {
    nbX[nb_count] = posX + 2;
    nbY[nb_count] = posY;
    nb_count++;
  }
  if (nb_S(posX, posY) == 1) {
    nbX[nb_count] = posX;
    nbY[nb_count] = posY + 2;
    nb_count++;
  }
  if (nb_W(posX, posY) == 1) {
    nbX[nb_count] = posX - 2;
    nbY[nb_count] = posY;
    nb_count++;
  }
}

void fillMiddleCell(neighbor) {
  if (nbY[neighbor] - posY == -2) { // north neighbour chosen 
    maze[posX][posY - 1] = 1;
  }
  else if (nbX[neighbor] - posX == 2) { // east
    maze[posX + 1][posY] = 1;
  }
  else if (nbY[neighbor] - posY == 2) { // south
    maze[posX][posY + 1] = 1;
  }
  else if (nbX[neighbor] - posX == -2) { // west
    maze[posX - 1][posY] = 1;
  }
}

int loop_times = (maze_size + 1) / 2 * (maze_size + 1) / 2;
int calcLoopTimes() {
  if (maze_size % 2 == 0) {
    loop_times = ((maze_size) / 2) * ((maze_size) / 2);
  }
  else if (maze_size % 2 == 1) {
    loop_times = ((maze_size + 1) / 2) * ((maze_size + 1) / 2);
  }
  return loop_times;
}

// for even-sized mazes, add final 2 cells in corner to maze passage
void finishEvenMaze() {
  if (maze_size % 2 == 0) {
    int end_rand = rand() % 2;
    if (end_rand == 0) {
      maze[maze_size - 2][maze_size - 1] = 1;
      maze[maze_size - 1][maze_size - 1] = 1;
    }
    else if (end_rand == 1) {
      maze[maze_size - 1][maze_size - 2] = 1;
      maze[maze_size - 1][maze_size - 1] = 1;
    }
  }
}

void setFirstPoint() {
  // choose point (0,0) as first square in the maze
  posX = 0;
  posY = 0;
  maze[0][0] = 1;
  visitedX[0] = 0;
  visitedY[0] = 0;
  visited = 1;
  pushX(0);
  pushY(0);
}

void generateMaze() {
  setBaseMaze();
  setFirstPoint();
  
  while (visited < calcLoopTimes()) {
    // calculate all valid neighbouring cells
    getNeighbors();
    // check if all neighbours have been visited before
    if (nb_count == 0) {
      posX = popX(); // go back to last visited cell
      posY = popY();
      nb_count = 0;
    }
    else {
      int nb_rand = rand() % (nb_count);
      maze[nbX[nb_rand]][nbY[nb_rand]] = 1; // update randomly selected cell from wall (0) to passage (1)
      fillMiddleCell(nb_rand);

      posX = nbX[nb_rand]; // update new position
      posY = nbY[nb_rand];
      visitedX[visited] = posX; // keep track of visited cells
      visitedY[visited] = posY;
      pushX(posX); // add new cell to stack
      pushY(posY);
      nb_count = 0; // reset number of neighbours to 0
      visited++; // update count of visited cells
    }
  }
  finishEvenMaze();
}

const int width = 1000;
const int height = 1000;

const int waitTime = 75;
const int squareSize = 10;
const int stepSize = squareSize;
void drawBackground() {
  background();
  generateMaze();

  int xcord = 0; // RENAME THESE
  int ycord = 0;
  for (int col = 0; col < maze_size; col++) {
    for (int row = 0; row < maze_size; row++) {
      if (col == 0 && row == 0) {
        setColour(red);
      }
      else if (col == (maze_size - 1) && row == (maze_size - 1)) {
        setColour(green);
      }
      else if (maze[row][col] == 1) {
        setColour(lightgray);
      }
      else if (maze[row][col] == 0) {
        setColour(black);
      }
      fillRect(xcord, ycord, squareSize, squareSize);
      xcord += (squareSize);
    }
    xcord = 0;
    ycord += squareSize;
  }
}

// determines which direction the robot is facing
// multiples of 1 = East, 2 = South, 3 = West, 4 = North
int pos = 1001;
int canMoveForward() {
  if (pos % 4 == 1 && (posX + 1) < maze_size && maze[posX + 1][posY] == 1) { // East
    return 1;
  }
  else if (pos % 4 == 2 && (posY + 1) < maze_size && maze[posX][posY + 1] == 1) { // South
    return 1;
  }
  else if (pos % 4 == 3 && (posX - 1) > -1 && maze[posX - 1][posY] == 1) { // West
    return 1;
  }
  else if (pos % 4 == 0 && (posY - 1) > -1 && maze[posX][posY - 1] == 1) { // North
    return 1;
  }
  else {
    return 0;
  }
}

int canMoveRight() {
  if (pos % 4 == 1 && (posY + 1) < maze_size && maze[posX][posY + 1] == 1) { // East
    return 1;
  }
  else if (pos % 4 == 2 && (posX - 1) > -1 && maze[posX - 1][posY] == 1) { // South
    return 1;
  }
  else if (pos % 4 == 3 && (posY - 1) > -1 && maze[posX][posY - 1] == 1) { // West
    return 1;
  }
  else if (pos % 4 == 0 && (posX + 1) < maze_size && maze[posX + 1][posY] == 1) { // North
    return 1;
  }
  else {
    return 0;
  }
}

int canMoveLeft()
{
  if (pos % 4 == 1 && (posY - 1) < maze_size && maze[posX][posY - 1] == 1) { // East
    return 1;
  }
  else if (pos % 4 == 2 && (posX + 1) < maze_size && maze[posX + 1][posY] == 1) { // South
    return 1;
  }
  else if (pos % 4 == 3 && (posY + 1) > -1 && maze[posX][posY + 1] == 1) { // West
    return 1;
  }
  else if (pos % 4 == 0 && (posX - 1) > -1 && maze[posX - 1][posY] == 1) { // North
    return 1;
  }
  else {
    return 0;
  }
}

// (x,y) coordinates for the three corners of the robot
int robotX[] = {0, 0, squareSize};
int robotY[] = {0, squareSize, squareSize / 2};
void forward(int step) {
  clear();
  if (canMoveForward() == 1) {
    if (pos % 4 == 1) { // East
      for (int i = 0; i < 3; i++) {
        robotX[i] += step;
      }
      posX += 1;
    }
    else if (pos % 4 == 2) { // South
      for (int i = 0; i < 3; i++) {
        robotY[i] += step;
      }
      posY += 1;
    }
    else if (pos % 4 == 3) { // West
      for (int i = 0; i < 3; i++) {
        robotX[i] -= step;
      }
      posX -= 1;
    }
    else if (pos % 4 == 0) { // North
      for (int i = 0; i < 3; i++) {
        robotY[i] -= step;
      }
      posY -= 1;
    }
  }
  fillPolygon(3, robotX, robotY);
}

void turn_right(void) {
  clear();
  if (pos % 4 == 1) { // if robot facing East
    robotX[0] += squareSize;
    robotX[2] -= squareSize / 2;
    robotY[1] -= squareSize;
    robotY[2] += squareSize / 2;
  }
  else if (pos % 4 == 2) { // if robot facing South
    robotX[1] += squareSize;
    robotX[2] -= squareSize / 2;
    robotY[0] += squareSize;
    robotY[2] -= squareSize / 2;
  }
  else if (pos % 4 == 3) { // if robot facing West
    robotX[0] -= squareSize;
    robotX[2] += squareSize / 2;
    robotY[1] += squareSize;
    robotY[2] -= squareSize / 2;
  }
  else if (pos % 4 == 0) { // if robot facing North
    robotX[1] -= squareSize;
    robotX[2] += squareSize / 2;
    robotY[0] -= squareSize;
    robotY[2] += squareSize / 2;
  }
  fillPolygon(3, robotX, robotY);
  pos++;
}

void turn_left(void) {
  clear();
  if (pos % 4 == 1) { // if robot facing East
    robotX[1] += squareSize;
    robotX[2] -= squareSize / 2;
    robotY[0] += squareSize;
    robotY[2] -= squareSize / 2;
  }
  else if (pos % 4 == 2) { // if robot facing South
    robotX[0] -= squareSize;
    robotX[2] += squareSize / 2;
    robotY[1] += squareSize;
    robotY[2] -= squareSize / 2;
  }
  else if (pos % 4 == 3) { // if robot facing West
    robotX[1] -= squareSize;
    robotX[2] += squareSize / 2;
    robotY[0] -= squareSize;
    robotY[2] += squareSize / 2;
  }
  else if (pos % 4 == 0) { // if robot facing North
    robotX[0] += squareSize;
    robotX[2] -= squareSize / 2;
    robotY[1] -= squareSize;
    robotY[2] += squareSize / 2;
  }
  fillPolygon(3, robotX, robotY);
  pos--;
}

void solveMaze() {
  foreground();
  setColour(magenta);
  posX = 0;
  posY = 0;
  fillPolygon(3, robotX, robotY);

  while ((posX + posY) != (maze_size + maze_size - 2)) {
    if (canMoveRight() == 1) {
      turn_right();
      sleep(waitTime);
      forward(stepSize);
      sleep(waitTime);
    }
    else if (canMoveForward() == 1) {
      forward(stepSize);
      sleep(waitTime);
    }
    else {
      turn_left();
      sleep(waitTime);
    }
  }
}

int main(void) {
  setWindowSize(width, height);
  drawBackground();
  solveMaze();
  return 0;
}

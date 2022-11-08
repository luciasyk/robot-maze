# Maze Generation and Maze Solver in C

The program randomly generates a maze using a backtracking algorithm, and then solves the maze using a wall follower algorithm (right hand rule). 

## Maze Generation

The top left square is chosen as the first cell in the maze passage. Then, an adjacent neighbour at a distance 2 cells away from the original cell is randomly selected. The chosen neighbour cell, as well as the cell that lies between the first and neighbour cell, are added to the maze passage. The process repeats until there are no adjacent neighbour cells. The algorithm then backtracks through the created maze passage until there are adjacent neighbour cells. For an nxn grid, the algorithm ends when (n/2)^2 neighbour cells have been visited for even values of n and when (n+1/2)^2 neighbour cells have been visited for odd values of n. 

### Maze Solving

To solve the maze, the robot is instructed to: 1) turn right if it is possible, 2) move forward if it is not possible to turn right, and 3) turn left if it is not possible to turn right or to move forward. The robot always travels 'hugging' a wall on its right side. 

#### Commands to Run
```bash
gcc main.c graphics.c
./a.out | java -jar drawapp-2.0.jar
```

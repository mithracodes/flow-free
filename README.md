# flow-free

This is my submission for ***Project 3 of COMP20003 Algorithms and Data Structures in Sem 2 2022***. This program involves the implementation of a solver for the game Flow Free, a puzzle published by the professional puzzler Sam Loyd, in a column he wrote for the Brooklyn Daily Eagle in 1897. Some of the code is built upon the open-source terminal version made available by mzucker.

## Overview

The game presents a grid with colored dots occupying some of the squares. The objective is to connect dots of the same color by drawing pipes between them such that the entire grid is occupied by pipes. However, pipes may not intersect. 

The difficulty is determined by the size of the grid, ranging from 5x5 to 15x15 squares.

![](flowfree.gif)

The Flow Free Solver is a program that finds the shortest path to solve a Flow Free puzzle. Each possible configuration of the grid is called a state, and the Flow Free Graph *G = ⟨ V, E ⟩* is implicitly defined. The vertex set *V* is defined as all the possible configurations (states), and the edges *E* connecting two vertexes are defined by the legal movements (right, left, up, down) for each color. All edges have a weight of 1.

## The Algorithm

The program uses Dijkstra to find the shortest path to the solution, along with some game-specific optimizations to speed up the algorithm. The initial node (vertex) corresponds to the initial configuration, and the algorithm selects a node to expand along with an ordering of which is the next color the algorithm should try to connect. Once all the children have been added to the priority queue, the algorithm will pick again the next node to extend, as well as the next color to consider, until a solution is found.

Different color orderings can be tried with the command line options (type `./flow -h` to see all options). Each ordering has a different impact on the number of nodes needed by Dijkstra to find a solution. By default, the search will expand a maximum of 1GB of nodes.

### Applying moves

When applyMoveDirection is applied, the program create a new node that points to the parent, updates the grid resulting from applying the direction chosen, updates the priority of the node, and updates any other auxiliary data in the node. The priority of the node is given by the length of the path from the root node, i.e. how many grid cells have been painted. 

### Dead-ends

A dead-end is a configuration for which you know a solution cannot exist. The figure above shows an example of a dead-end: the cell marked with X cannot be filled with any color. You can recognize dead-end cells by looking for a free cell in the grid that is surrounded by three completed path segments (colored) or walls. The current position of an in-progress pipe and goal position have to be treated as free space. There might be multiple paths leading to a solution. Hence. the algorithm considers the possible actions in the following order: left (0), right (1), up (2) or down (3). 

## Input

To run the Flow Free solver, execute the following command:

```css
./flow [options]  <puzzleName1> ... <puzzleNameN> 
```
For example, running the solver for the "regular_5x5_01.txt" puzzle can be done by executing the following command:

```bash
 ./flow puzzles/regular_5x5_01.txt
 ```
 
The program will report whether the search was successful, the number of nodes generated, and the time taken. By default, the program reports the solutions in a concise format. However, the `-q` flag can be used to report the solutions even more concisely. This option can be useful for running several puzzles at once and studying their performance.

If the `-A` option is appended, the program will animate the solution found. If the `-d` option is appended, the program will use the dead-end detection mechanism that was implemented. Other options can be explored, such as the ordering in which the colors are explored. By default, the program considers the color that has fewer free neighbors (most constrained) first.

All available options can be found by using the `-h` flag.

## Output

If the user includes the option -q, the program will print a summary of the search results for each puzzle provided as input, which includes:

1. Puzzle Name 
2. SearchFlag
3. Total Search Tim (in seconds)
4. Number of generated nodes
5. A final Summary

For example, the output of the solver  `./flow -q ../puzzles/regular_*` could be:

```powershell
../puzzles/regular_5x5_01.txt s 0.000 18
../puzzles/regular_6x6_01.txt s 0.000 283
../puzzles/regular_7x7_01.txt s 0.002 3,317
../puzzles/regular_8x8_01.txt s 0.284 409,726
../puzzles/regular_9x9_01.txt s 0.417 587,332
5 total s 0.704 1,000,676
```

These numbers depend on the implementation of the search, the ordering you use, and whether dead-ends are pruned.   If dead-end pruning is used by adding the -d flag, the program will output:

```powershell
../puzzles/regular_5x5_01.txt s 0.000 17
../puzzles/regular_6x6_01.txt s 0.000 254
../puzzles/regular_7x7_01.txt s 0.001 2,198
../puzzles/regular_8x8_01.txt s 0.137 182,136
../puzzles/regular_9x9_01.txt s 0.210 279,287
5 total s 0.349 463,892
```




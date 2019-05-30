Game Of Life: Multithreading
---
Spring of 2019

Programmer:
Nathan Hodgson

### Note

Program does not work on MacOS due to pthread_barrier limitations. 

### Program Description

Using multithreading, this program implements "Conway's Game of Life", a zero-player game which simulates the evolution of a grid of cells in a turn-by-turn basis. The user specifies a desired number of threads to be created, and the program allocates each thread a chunk of the board to calculate for each round. More information about "Conway's Game of Life" at: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

### Compilation Specifications

The following command line argument compiles the program:

```shell
make all
```

### Execution Specifications

Run main with the following command line arguments:
1. Name of input file (default oscillator.txt is given)
2. Number of desired threads
	- NOTE: Number of threads cannot exceed grid size. 
3. Row or column-wise partitioning of threads (row or col)
4. If you want to grid to be a torus or not (wrap or nowrap)
5. If you want to see each iteration of the game (show) or just the final grid state (hide)
6. If you chose show, specify speed of iterations with slow, med, or fast

### Two Sample Command Line Arguments

```shell
./GoL oscillator.txt 5 row wrap show slow
```
```shell
./GoL oscillator.txt 3 col nowrap hide
```

### Output

- If you chose to show the grid, each iteration will be printed ending with the final grid state
- If you chose to hide the grid, only the final grid state will be printed
- Total number of live cells processed during execution
- Total time taken for the number of iterations and grid size

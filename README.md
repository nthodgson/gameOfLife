Game Of Life
---
Spring of 2019

Programmer:
Nathan Hodgson

### Note

Program currently does not work on Mac OS.

### Program Description

This program implements "Conway's Game of Life", a zero-player game which simulates the evolution of a grid of cells in a turn-by-turn basis. More information at: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life

### Execution Specifications

Run GoL with three command line arguments:
1. Name of input file (default input.txt is given)
2. If you want to grid to be a torus or not (wrap or nowrap)
3. If you want to see each iteration of the game (show) or just the final grid state (hide)
4. If you chose show, specify speed of iterations with slow, med, or fast

### Two Sample Command Line Arguments

- ./GoL input.txt wrap show slow
- ./GoL input.txt nowrap hide

### Output

- If you chose to show the grid, each iteration will be printed ending with the final grid state
- If you chose to hide the grid, only the final grid state will be printed
- Total time taken for the number of iterations and grid size

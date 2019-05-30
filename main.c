#include "global.h"

// Programmer: Nathan Hodgson
// Program: main.c

int main(int args, char** arg) {
	int i = 0, rows = 0, cols = 0, iterations = 0, length = 0, sizeOfLine = 0;
	int **board, **newBoard;
	char fileName[50];
	char speed[10];
	strcpy(fileName, arg[1]);
	bool wrap, show, row;
	int numThreads = atoi(arg[2]);
	struct threadData threadArray[numThreads];
	struct gridData grid;

	config(speed, &wrap, &show, args, arg);
	populateBoard(&rows, &cols, &iterations, &length, &board, fileName);

	if (strcmp(arg[3], "row") == 0) { 
		row = true;
		sizeOfLine = rows;
	}
	else {
		row = false;
		sizeOfLine = cols;
	}

	divideThreads(numThreads, sizeOfLine, threadArray);

	newBoard = (int **)malloc(rows * sizeof(int *));
	for (i=0; i<rows; i++) { // Allocate size of matrix using rows value from input file
		newBoard[i] = (int *)malloc(cols * sizeof(int));
	}

	grid.numThreads = numThreads;
	grid.rows = rows;
	grid.cols = cols;
	grid.row = row;
	grid.show = show;
	grid.wrap = wrap;
	grid.board = board;
	grid.newBoard = newBoard;
	grid.iterations = iterations;

	playGame(board, newBoard, numThreads, rows, cols, iterations, wrap, show, speed, threadArray, grid);
	printThreadInfo (numThreads, row, rows,cols,threadArray);

	for (int i=0; i<rows; i++) { // Free dynamically allocated memory
		free(board[i]);
		free(newBoard[i]);
	}
	free(board);
	free(newBoard);
	
	pthread_mutex_destroy(&lock); // Destroy thread mutex and barrier
	pthread_barrier_destroy(&barrier);
	pthread_exit(NULL);
	
	return 0;
}


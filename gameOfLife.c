#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>

// Programmer: Nathan Hodgson
// Program: gameOfLife.c

// Declare struct for thread info

struct threadData { 
   int tid, startIndex, endIndex, numLines, numCols;
   int **threadBoard;
}; 

// Function prototype declarations

void populateBoard(int *rows, int *cols, int *iterations, int *length, int ***board, char fileName[]);
void printBoard(int **board, int rows, int cols);
void updateBoard(int **board, int **newBoard, int rows, int cols, bool wrap);
void config(char speed[], bool *wrap, bool *show, int args, char **arg);
void resetBoard(int **board, int **newBoard, int rows, int cols);
void playGame(int **board, int **newBoard, int rows, int cols, int iterations, bool wrap, bool show, char speed[]);
void divideThreads(int numThreads, int sizeOfLine, struct threadData threadArray[numThreads]);
void printThreadInfo (int numThreads, bool row, int rows, int cols, struct threadData threadArray[numThreads]);

int main(int args, char** arg) {
	int i = 0, rows = 0, cols = 0, iterations = 0, length = 0, sizeOfLine = 0;
	int **board, **newBoard;
	char fileName[50];
	char speed[10];
	strcpy(fileName, arg[1]);
	bool wrap, show, row;
	int numThreads = atoi(arg[2]);
	struct threadData threadArray[numThreads];

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

	playGame(board, newBoard, rows, cols, iterations, wrap, show, speed);

	for (int i=0; i<rows; i++) { // Free dynamically allocated memory
		free(board[i]);
		free(newBoard[i]);
	}
	free(board);
	free(newBoard);

	return 0;
}

/* ====================================================================================
playGame(): Core function of the game. Accepts number of iterations as a parameter
and executes the full game. Prints only the final board if bool show = false. Prints
each iteration if show = true. 
==================================================================================== */

void playGame(int **board, int **newBoard, int rows, int cols, int iterations, bool wrap, bool show, char speed[]) { 
	struct timeval start_time, end_time;
	int sleepTime, i;

	if (show) {
		if (strcmp(speed, "slow") == 0) {
			sleepTime = 333333;
		} else if (strcmp(speed, "med") == 0) {
			sleepTime = 100000;
		} else {
			sleepTime = 33333;
		}
	} 

	gettimeofday(&start_time, NULL);

	for (i=0; i<iterations; i++) { // Runs specified number of iterations of the game
		updateBoard(board, newBoard, rows, cols, wrap);
		resetBoard(board, newBoard, rows, cols);
		if (show) {
			system("clear");
			printBoard(board, rows, cols);
			usleep(sleepTime);
		}
	}


	if (!show) {
		printBoard(board, rows, cols);
	}

	gettimeofday(&end_time, NULL);

	printf("\nTotal time for %d iterations of %dx%d is %f seconds.\n\n", iterations, rows, cols, ((((double)end_time.tv_sec)
		  - ((double)start_time.tv_sec))+((float)end_time.tv_usec-(float)start_time.tv_usec)/1000000.0));

	return;
}

/* ====================================================================================
populateBoard(): Initializes the 'board' matrix by reading in the input file and
storing the coordinates of 'live cells' in the board. Indices that are not 'live cells'
will contain 0s, while live cells will contain 1s. 
==================================================================================== */

void populateBoard(int *rows, int *cols, int *iterations, int *length, int ***board, char fileName[]) { 
	int buffer[100], rowBuf[100], colBuf[100];
	int colCount = 0, rowCount = 0, i = 0, j = 0;

	FILE* infile;
	infile = fopen(fileName, "r");
	if (infile == NULL) {
		printf("Invalid file name... Please try again.\n");
		exit(1);
	}
	else {
		fscanf(infile, "%d", buffer);
		*rows = *buffer;
		fscanf(infile, "%d", buffer);
		*cols = *buffer;
		fscanf(infile, "%d", buffer);
		*iterations = *buffer;

		int liveRows[*rows];
		int liveCols[*cols];

		*board = (int **)malloc(*rows * sizeof(int *));
		for (i=0; i<*rows; i++) { // Allocate size of matrix using rows value from input file
			(*board)[i] = (int *)malloc(*cols * sizeof(int));
		}

		for (i=0; i<*rows; i++) { // Populate board with 0's
			for (j=0; j<*cols; j++) {
					(*board)[i][j] = 0;
			}
		}

		while (fscanf(infile, "%d %d", rowBuf, colBuf) == 2) { // Populate matrix with live indices
			(*board)[*rowBuf][*colBuf] = 1; 
			*length += 1;
		}

		fclose(infile);
	}
	return;
}

/* ====================================================================================
updateBoard(): Runs through one iteration of cell checking. Updates 'newBoard' based 
on current values in 'board'
==================================================================================== */

void updateBoard(int **board, int **newBoard, int rows, int cols, bool wrap) { 
	int i = 0, j = 0, n = 0, x = 0, torusN = 0, torusX = 0, count = 0;

	// This quadruple-nested for-loop runs through every index in 'board'
	// and checks the 8 indexes around it based on the 'wrap' condition. 

	for (i=0; i<rows; i++) { 
		for (j=0; j<cols; j++) {
			for (n=(i-1); n<(i+2); n++) {
				for (x=(j-1); x<(j+2); x++) {
					if ((n >= 0 && n < rows) && (x >= 0 && x < cols)) {
						if (n != i || x != j) {
							if (board[n][x] == 1)
								count++;
						}
					} else if (wrap) {
						if (n == -1)
							torusN = rows-1;
						else if (n == rows)
							torusN = 0;
						else
							torusN = n;
						if (x == -1)
							torusX = cols-1;
						else if (x == cols)
							torusX = 0;
						else
							torusX = x;
						if (board[torusN][torusX] == 1) 
							count++;
					}
				}
			}
			if ((count == 2) && (board[i][j] == 1)) // Updates newBoard based on live cells
				newBoard[i][j] = 1;
			else if (count == 3) 
				newBoard[i][j] = 1;
			else
				newBoard[i][j] = 0;
			count = 0;
		}
	}

	return;
}

/* ====================================================================================
resetBoard(): Makes board equal to newBoard for each iteration of playGame()
==================================================================================== */

void resetBoard(int **board, int **newBoard, int rows, int cols) {  
	int i, j;

	for (i=0; i<rows; i++) {
		for (j=0; j<cols; j++) {
			board[i][j] = newBoard[i][j];
		}
	}

	return;
}

/* ====================================================================================
printBoard(): Prints a board of size "rows" to the console. Board must be of type int**
==================================================================================== */

void printBoard(int **board, int rows, int cols) { // Prints the board
	int i = 0, j = 0;
	printf("\n\n");
	for (i=0; i<rows; i++) {
		for (j=0; j<cols; j++) {
			if (board[i][j] == 1)
				printf(" & ");
			else 
				printf(" - ");
		}
		printf("\n");
	}
	return;
}

/* ====================================================================================
config(): Reads in all config settings and checks for command line errors. Array 
speed[] must have at least size 4. 
==================================================================================== */

void config(char speed[], bool *wrap, bool *show, int args, char **arg) { 

	if (args > 5 || args < 4) {
		printf("Invalid input... Exiting the program.\n");
		exit(1);
	}

	if ((strcmp(arg[2],"wrap") != 0) && (strcmp(arg[2],"nowrap") != 0)) {
		printf("Incorrect wrap setting... Please try wrap or nowrap.\n");
		exit(1);
	} else if ((strcmp(arg[3],"show") != 0) && (strcmp(arg[3],"hide") != 0)) {
		printf("Incorrect show setting... Please try show or hide.\n");
		exit(1);
	} else if (strcmp(arg[3], "show") == 0) {
		*show = true;
		strcpy(speed, arg[4]);
		if ((strcmp(arg[4],"slow") != 0) && (strcmp(arg[4],"med") != 0) && (strcmp(arg[4],"fast") != 0)) {
			printf("Incorrect speed setting... Please try slow, med, or fast.\n");
			exit(1);
		}
	} else {
		*show = false;
	}

	if (strcmp(arg[2], "wrap") == 0) {
		*wrap = true; 
	} else {
		*wrap = false;
	}

	if (!*show && args == 5) {
		printf("Speed setting entered on 'hide', exiting...\n");
		exit(1);
	}

	return;
}

/* ====================================================================================
divideThreads(): Determines how many rows or columns each thread should be allocated
and stores that value in each thread's struct index. Starting and ending index for 
each row and column are also stored in each thread's struct index. 
==================================================================================== */

void divideThreads(int numThreads, int sizeOfLine, struct threadData threadArray[numThreads]) {

	int linesPerThread = 0;
	int numLarge = sizeOfLine % numThreads;
	int numSmall = numThreads - numLarge;
	int largeThread = ((sizeOfLine/numThreads) + (sizeOfLine % numThreads != 0));

	if (sizeOfLine % numThreads == 0) {
		linesPerThread = sizeOfLine/numThreads;
		for (int i=0; i<numThreads; i++) { // If number of threads can be evenly distributed, assign each thread an equal size
			threadArray[i].numLines = linesPerThread;
			threadArray[i].startIndex = linesPerThread * i;
			threadArray[i].endIndex = threadArray[i].startIndex + (numThreads-1);
		}
	}
	else {
		for (int i=0; i<numLarge; i++) { // Calculates how many large threads are needed and assigns them a number of rows or cols, start, and end point
			threadArray[i].numLines = largeThread;
			threadArray[i].startIndex = largeThread * i;
			threadArray[i].endIndex = threadArray[i].startIndex + (largeThread-1);
		}
		for (int j=0; j<numSmall; j++) { // Calculates how many small threads are needed and assigns them a number of rows or cols, start, and end point
			threadArray[j+numLarge].numLines = (sizeOfLine/numThreads);
			threadArray[j+numLarge].startIndex = threadArray[j+numLarge-1].endIndex + 1;
			threadArray[j+numLarge].endIndex = threadArray[j+numLarge].startIndex + ((sizeOfLine/numThreads)-1);
		}
	}

	return;
}

/* ====================================================================================
printThreadInfo(): Prints thread information based on contents of the struct threadArray
and additional parameters provided.
==================================================================================== */

void printThreadInfo (int numThreads, bool row, int rows, int cols, struct threadData threadArray[numThreads]) {

	for (int i=0; i<numThreads; i++) {
		if (row) {
			printf("tid: %d  rows: %d:%d  (%d)  cols: %d:%d  (%d)\n", i, threadArray[i].startIndex, threadArray[i].endIndex, threadArray[i].numLines, 
				0, cols-1, cols);
		}
		else {
			printf("tid: %d  rows: %d:%d  (%d)  cols: %d:%d  (%d)\n", i, 0, rows-1, rows, threadArray[i].startIndex, threadArray[i].endIndex, 
				threadArray[i].numLines);
		}
	}

	return; 
}

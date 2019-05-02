#ifndef __GLOBAL_H_
#define __GLOBAL_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h> 

// Programmer: Nathan Hodgson
// Program: global.h

// Declare struct for thread info

struct threadData { 
   int i, tid;
   int startIndex, endIndex, numLines;
   int **board, **newBoard;
   struct gridData *gridPointer;
}; 

// Declare struct for grid data

struct gridData {
	int sleepTime, rows, cols, numThreads, iterations;
	int **board, **newBoard;
	bool row, show, wrap;
};

// pthread barrier and mutex declarations

pthread_barrier_t barrier;
pthread_mutex_t lock;

// Function prototype declarations

void populateBoard(int *rows, int *cols, int *iterations, int *length, int ***board, char fileName[]);
void printBoard(int **board, int rows, int cols);
void updateBoard(int **board, int **newBoard, int startRows, int endRows, int startCols, int endCols, int rows, int cols, bool wrap);
void config(char speed[], bool *wrap, bool *show, int args, char **arg);
void resetBoard(int **board, int **newBoard, int startRows, int endRows, int startCols, int endCols);
void playGame(int **board, int **newBoard, int numThreads, int rows, int cols, int iterations, bool wrap, bool show, char speed[], 
	struct threadData threadArray[numThreads], struct gridData grid);
void divideThreads(int numThreads, int sizeOfLine, struct threadData threadArray[numThreads]);
void printThreadInfo (int numThreads, bool row, int rows, int cols, struct threadData threadArray[numThreads]);
void *runThreads(void *threadArray);

#endif
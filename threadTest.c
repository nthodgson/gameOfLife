#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h> 

// Let us create a global variable to change it in threads 
int g = 0; 
  
// The function to be executed by all threads 
void *myThreadFun(void *vargp) 
{ 
    // Store the value argument passed to this thread 
    int *myid = (int *)vargp; 
  
    // Let us create a static variable to observe its changes 
    static int s = 0; 
  
    // Change static and global variables 
    ++s; ++g; 
  
    // Print the argument, static and global variables 
    printf("Thread ID: %d, Static: %d, Global: %d\n", *myid, ++s, ++g); 
} 
  
int main() 
{ 
    int i; 
    pthread_t tid[3];
  
    // Let us create three threads 
    for (i = 0; i < 3; i++) 
        pthread_create(&tid[i], NULL, myThreadFun, (void *)&tid[i]); 
  	printf("Here\n");
    pthread_exit(NULL); 
    return 0; 
} 


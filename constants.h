#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#define THREADSIZE 18
#define calcfuncsize 6
#define directionsize 28

int marble_result,oneslen,genstep,area,userframe,pcframe,runningthreadsize;
int board2[8][8];
int** ones;
bool** checked;
bool running[THREADSIZE-calcfuncsize];

FILE* file ;


pthread_mutex_t mutexgeneral;
pthread_mutex_t mutexcalcrunning;
pthread_mutex_t filemutex;
pthread_t threads[THREADSIZE-calcfuncsize];
extern int directions[28][2];
extern int frames[17][13];

typedef struct {
    int color;
    int** board;
}Data2;

typedef struct {
    int frame;
}Node;

extern Node* newnode[8][8];

typedef struct {
    int x;
    int y;
    int step;
    int not_x;
    int not_y;
    int color;
    bool ret;
    int** board;
    int* result;
    int index;
}Data;

void initialize();
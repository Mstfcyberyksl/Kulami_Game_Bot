#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#define THREADSIZE 18
#define calcfuncsize 6
#define directionsize 28

extern int marble_result,oneslen,genstep,area,userframe,pcframe,runningthreadsize;
extern int board2[8][8];
extern int** ones;
extern bool** checked;
extern bool running[THREADSIZE-calcfuncsize];
int which(int x, int y);
extern FILE* file ;


extern pthread_mutex_t mutexgeneral;
extern pthread_mutex_t mutexcalcrunning;
extern pthread_mutex_t filemutex;
extern pthread_t threads[THREADSIZE-calcfuncsize];
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
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "constants.h"

#define THREADSIZE 18
#define calcfuncsize 6
#define directionsize 28

int oneslen = 0;
int genstep = -1;
int area = 0;
int userframe = -1;
int pcframe = -1;
int runningthreadsize = 0;
//pthread_mutex_t filemutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t threads[THREADSIZE-calcfuncsize];

pthread_mutex_t mutexgeneral;
pthread_mutex_t mutexcalcrunning;
pthread_mutex_t filemutex;

int directions[28][2] = {
    {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7},
    {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0},
    {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}
};

int frames[17][13] = {{4,0,0,0,1,1,0,1,1,-1,-1,-1,-1},
                    {6,0,2,0,3,0,4,1,2,1,3,1,4},
                    {6,0,5,0,6,1,5,1,6,2,5,2,6},
                    {3,0,7,1,7,2,7,-1,-1,-1,-1,-1,-1},
                    {2,2,0,3,0,-1,-1,-1,-1,-1,-1,-1,-1},
                    {4,2,1,2,2,3,1,3,2,-1,-1,-1,-1},
                    {2,2,3,2,4,-1,-1,-1,-1,-1,-1,-1,-1},
                    {3,4,0,4,1,4,2,-1,-1,-1,-1,-1,-1},
                    {6,3,3,3,4,3,5,4,3,4,4,4,5},
                    {2,3,6,3,7,-1,-1,-1,-1,-1,-1,-1,-1},
                    {4,4,6,4,7,5,6,5,7,-1,-1,-1,-1},
                    {6,5,0,5,1,6,0,6,1,7,0,7,1},
                    {2,5,2,6,2,-1,-1,-1,-1,-1,-1,-1,-1},
                    {4,5,3,5,4,6,3,6,4,-1,-1,-1,-1},
                    {3,5,5,6,5,7,5,-1,-1,-1,-1,-1,-1},
                    {3,7,2,7,3,7,4,-1,-1,-1,-1,-1,-1},
                    {4,6,6,6,7,7,6,7,7,-1,-1,-1,-1}};


void initialize(){
    int i,j;
    for (i = 0;i < 8; i++){
        for (j = 0;j < 8;j++){
            board2[i][j] = 0;
        }
    }
    checked = (bool**)malloc(8 * sizeof(bool*));
    for (i = 0;i < 8;i++){
        checked[i] = (bool*)malloc(8 * sizeof(bool));
        for (j = 0;j < 8;j++){
            checked[i][j] = false;
        }
    }
    for (i = 0;i < 8;i++){
        for(j = 0;j < 8;j++){
            newnode[i][j] = (Node*)malloc(sizeof(Node));
            newnode[i][j]->frame = which(i,j);
        }
    }

    ones = (int**)malloc(1 * sizeof(int*));
}

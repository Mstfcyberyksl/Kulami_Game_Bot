#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "constants.h"

void* horizontal_points(void *arg);
void* vertical_points(void *arg);
void* diagonal_points_45(void *arg);
void* diagonal_points_135(void *arg);
int dfs(int i,int j,int color,int (*board)[8]);
void remove2(int x, int y);
void helper(int x,int y,int color, int** board);
void* marble_area_points(void *arg);
void* place_area_points(void *arg);
Data2 copydata2(Data2* data);
Data copydata(Data* data);
void freedata2(Data2 data);
int* calculate(int color, int** board);
//int which(int x, int y);
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "constants.h"
#include "calculations.h"


void* horizontal_points(void *arg){
    int i,j,length = 0,pc = 0,user = 0;

    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        perror("Failed to allocate memory horizontal");
        pthread_exit(NULL);
    }

    Data2* data = (Data2*)arg;
    int** board = (int**)malloc(8 * sizeof(int*));
    for (i = 0;i < 8;i++){
        board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(board[i],data->board[i],8 * sizeof(int));
    } 

    int color = data->color;

    for (i = 0;i < 8;i++){
        for (j = 1;j < 8;j++){
            
            if (j-1 > -1 && board[i][j-1] == board[i][j]){
                length++;
            }
            else{
                if (length >= 5){
                    if (board[i][j-1] == 1){
                        user += length;
                    }
                    else if (board[i][j-1] == 2){
                        pc += length;
                    }
                }
                length = 1;
            }
        }

        if (length >= 5){
            if (board[i][7] == 1){
                user += length;
            }
            else{
                pc += length;
            }
        }
        length = 1;
        
    }
    *result = pc - user;
    
    
    for (i = 0;i < 8;i++){
        free(board[i]);
    }
    free(board);
    return (void*)result;
}
void* vertical_points(void *arg){
    int i,j,length = 0,pc = 0,user = 0;

    Data2* data = (Data2*)arg;
    int** board = (int**)malloc(8 * sizeof(int*));
    for (i = 0;i < 8;i++){
        board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(board[i],data->board[i],8 * sizeof(int));
    } 
    
    int color = data->color;

    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        perror("Failed to allocate memory vertical");
        pthread_exit(NULL);
    }

    for (j = 0;j < 8;j++){
        for (i = 1;i < 8;i++){
            
            if (i-1 > -1 && board[i-1][j] == board[i][j]){
                length++;
            }
            else{
                if (length >= 5){
                    if (board[i-1][j] == 1){
                        user += length;
                    }
                    else if (board[i-1][j] == 2){
                        pc += length;
                    }
                }
                length = 1;
            }
        }

        if (length >= 5){
            if (board[7][j] == 1){
                user += length;
            }
            else{
                pc += length;
            }
        }
        length = 1;
        
    }
   
    *result = pc - user;
    
    for (i = 0;i < 8;i++){
        free(board[i]);
    }
    free(board);
    return (void*)result;
}
void* diagonal_points_45(void *arg){
    int i , length = 0, pc = 0, user = 0,x,y;
    int places[7][2] = {
        {7,0},{6,0},{5,0},{4,0},{7,3},{7,2},{7,1}
    };

    Data2* data = (Data2*)arg;
    int** board = (int**)malloc(8 * sizeof(int*));
    for (i = 0;i < 8;i++){
        board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(board[i],data->board[i],8 * sizeof(int));
    } 
    int color = data->color;

    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        perror("Failed to allocate memory 45");
        pthread_exit(NULL);
    }


    for(i = 0;i < 7;i++){
        x = places[i][0];
        y = places[i][1];
        length = 1;
        while (x < 8 && y < 8 && x > -1 && y > -1){
            if (x+1 < 8 && x+1 > -1 && y-1 < 8 && y-1 > -1 &&  board[x][y] == board[x+1][y-1]){
                length++;
            }
            else{
                if (length >= 5){
                    if (x+1 < 8 && x+1 > -1 && y-1 < 8 && y-1 > -1 && board[x+1][y-1] == 2){
                        pc += length;
                    }
                    else{
                        user += length;
                    }
                }
                length = 1;
            }
            x--;
            y++;
        }
        if (length >= 5){
            x++;
            y--;
            if (board[x][y] == 1){
                user += length;
            }
            else{
                pc += length;
            }
        }
    }

    *result = pc - user;
    for (i = 0;i < 8;i++){
        free(board[i]);
    }
    free(board);
    return (void*)result;
    
}
void* diagonal_points_135(void *arg){
    int i, length = 0, pc = 0, user = 0,x,y;
    int places[7][2] = {
        {7,4},{6,7},{5,7},{4,7},{7,5},{7,6},{7,7}
    };

    Data2* data = (Data2*)arg;
    int** board = (int**)malloc(8 * sizeof(int*));
    for (i = 0;i < 8;i++){
        board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(board[i],data->board[i],8 * sizeof(int));
    } 
    int color = data->color;

    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        perror("Failed to allocate memory 135");
        pthread_exit(NULL);
    }
    

    for(i = 0;i < 7;i++){
        x = places[i][0];
        y = places[i][1];
        length = 0;
        while (x > -1 && y > -1){
            if (x+1 < 8 && x+1 > -1 && y+1 < 8 && y+1 > -1 && board[x][y] == board[x+1][y+1]){
                length++;
            }
            else{
                if (length >= 5){
                    if (board[x+1][y+1] == 2){
                        pc += length;
                    }
                    else{
                        user += length;
                    }
                }
                length = 1;
                    
                
            }
            
            x--;
            y--;
        }
        if (length >= 5){
            x++;
            y++;
            if (board[x][y] == 1){
                user += length;
            }
            else{
                pc += length;
            }
        }
    }

    *result = pc - user;
    for (i = 0;i < 8;i++){
        free(board[i]);
    }
    free(board);
    return (void*)result;
}
int dfs(int i,int j,int color,int (*board)[8]){
    int k,m,n,temp = 0;
    
    checked[i][j] = true;
    marble_result++;
    
    k = i;
    m = j;
    for(n = 0;n < directionsize;n++){
        if (k + directions[n][0] < 8 && 
            k + directions[n][0] > -1 && 
            m + directions[n][1] < 8 &&
            m + directions[n][1] > -1 && 
            board[k + directions[n][0]][m + directions[n][1]] == color && 
            !checked[k + directions[n][0]][m + directions[n][1]]){
            temp += dfs(k + directions[n][0],m + directions[n][1],color,board);
        }
    }

    return temp+1;

}

void remove2(int x, int y){
    int i, j;
    for (i = 0;i < oneslen;i++){
        if (ones[i][0] == x && ones[i][1] == y){
            while (i < oneslen-1){
                ones[i][0] = ones[i+1][0];
                ones[i][1] = ones[i+1][1];
                i++;
            }
            if (oneslen > 1){
            ones = (int**)realloc(ones,(oneslen-1) * sizeof(int*));
            }
            else if (oneslen == 1){
                free(ones);
                ones = NULL;
            }
            oneslen--;
            break;
        }
    }
}

void helper(int x,int y,int color, int** board){
    area++;
    int directions[4][2] = {
        {1,0},{0,1},{-1,0},{0,-1}
    };

    for (int m = 0; m < 4;m++){
        if (x + directions[m][0] < 8 && 
            x + directions[m][0] > -1 && 
            y + directions[m][1] > -1 && 
            y + directions[m][1] < 8 && 
            board[x + directions[m][0]][y + directions[m][1]] == color){
            if (oneslen > 1){
            remove2(x + directions[m][0],y + directions[m][1]);
            helper(x + directions[m][0],y + directions[m][1],color,board);
        }
        }
    }
}

void* marble_area_points(void *arg){
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    Data2* data = (Data2*)arg;
    int i, j;
    int** board = (int**)malloc(8 * sizeof(int*));
    for (i = 0;i < 8;i++){
        board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(board[i],data->board[i],8 * sizeof(int));
    } 

    int color = data->color;

    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        perror("Failed to allocate memory marble area");
        pthread_exit(NULL);
    }
    
    oneslen = 0;
    for (i = 0;i < 8;i++){
        for (j = 0;j < 8;j++){
            if (board[i][j] == color){
                oneslen++;
                ones = (int**)realloc(ones,oneslen * sizeof(int*));
                ones[oneslen-1] = (int*)malloc(2 * sizeof(int)); 
                ones[oneslen-1][0] = i;
                ones[oneslen-1][1] = j;
            }
        }
    }
    
    int maximum = -1;
    int temp[2];

    while (oneslen > 0){
        area = 0;
        temp[0] = ones[oneslen-1][0];
        temp[1] = ones[oneslen-1][1];
        if (oneslen > 1){

            ones = (int**)realloc(ones,(oneslen-1) * sizeof(int*));
            oneslen--;

            helper(temp[0],temp[1],color,board);
        }
        else{
            area++;
            oneslen--;
        }
        if (area > maximum){
            maximum = area;
        }
    }

    *result = maximum;
    for (i = 0;i < 8;i++){
        free(board[i]);
    }
    free(board);
    pthread_mutex_unlock(&mutex);
    return (void*)result;   
}
    
    
void* place_area_points(void *arg){
    int i,j,pc = 0,user = 0,length = 0;
    int* result = (int*)malloc(sizeof(int));

    Data2* data = (Data2*)arg;
    int** board = (int**)malloc(8 * sizeof(int*));
    for (i = 0;i < 8;i++){
        board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(board[i],data->board[i],8 * sizeof(int));
    }
    int color = data->color;

    if (result == NULL) {
        perror("Failed to allocate memory place area");
        pthread_exit(NULL);
    }

    for (i = 0;i < 17;i++){
        pc = 0;
        user = 0;
        for (j = 1; j < frames[i][0]; j += 2){
            if (board[frames[i][j]][frames[i][j+1]] == 2){
                pc++;
            }
            else if (board[frames[i][j]][frames[i][j+1]] == 1){
                user++;
            }
        }
        if (pc > user){
            length += frames[i][0];
        }
        else if (user > pc){
            length -= frames[i][0];
        }
    }
    *result = length;
    for (i = 0;i < 8;i++){
        free(board[i]);
    }
    free(board);
    return (void*)result;
}

Data2 copydata2(Data2* data){
    Data2 copy = *data;

    copy.board = (int**)malloc(8 * sizeof(int*));
    for (int i = 0; i < 8; i++) {
        copy.board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(copy.board[i], data->board[i], 8 * sizeof(int));
    }    
    return copy;
}
Data copydata(Data* data){
    Data copy = *data;

    copy.board = (int**)malloc(8 * sizeof(int*));
    for (int i = 0; i < 8; i++) {
        copy.board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(copy.board[i], data->board[i], 8 * sizeof(int));
    }
    copy.result = (int*)malloc(33 * sizeof(int));
    memcpy(copy.result, data->result, 33 * sizeof(int));
    return copy;
}
void freedata2(Data2 data){
    for (int i = 0; i < 8; i++) {
        free(data.board[i]);
    }
    free(data.board);
}

int* calculate(int color, int** board){
    pthread_t* calcthreads = (pthread_t*)malloc(calcfuncsize * sizeof(pthread_t));
    int calcrunningsize = 0;
    int i;
    int** result = (int**)malloc(calcfuncsize * sizeof(int*));
    for(i = 0;i < calcfuncsize;i++){
        result[i] = (int*)malloc(sizeof(int));
    }
    int resultindex = 0;
    Data2* data = (Data2*)malloc(sizeof(Data2));
    data->color = color;
    data->board = (int**)malloc(8 * sizeof(int*));
    for (int i = 0;i < 8;i++){
        data->board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(data->board[i],board[i],8 * sizeof(int));
    }
    bool* yes = (bool*)malloc(calcfuncsize * sizeof(bool));
    for(i = 0;i < calcfuncsize;i++){
        yes[i] = true;
    }
    int ab; 
    Data2 parray[calcfuncsize];
    for(i = 0;i < calcfuncsize;i++){
        parray[i] = copydata2(data);
    }
    
    ab = pthread_create(&calcthreads[0],NULL,horizontal_points,(void*)&parray[0]);
    if (ab != 0){
        yes[0] = false;
        printf("THREAD CREATE ERROR\n");
    }
    
    ab = pthread_create(&calcthreads[1],NULL,vertical_points,(void*)&parray[1]);
    if (ab != 0){
        yes[1] = false;
        printf("THREAD CREATE ERROR\n");
    }
    
    ab = pthread_create(&calcthreads[2],NULL,diagonal_points_45,(void*)&parray[2]);
    if (ab != 0){
        yes[2] = false;
        printf("THREAD CREATE ERROR\n");
    }
    
    ab = pthread_create(&calcthreads[3],NULL,diagonal_points_135,(void*)&parray[3]);
    if (ab != 0){
        yes[3] = false;
        printf("THREAD CREATE ERROR\n");
    }
    
    ab = pthread_create(&calcthreads[4],NULL,place_area_points,(void*)&parray[4]);
    if (ab != 0){
        yes[4] = false;
        printf("THREAD CREATE ERROR\n");
    }
    
    ab = pthread_create(&calcthreads[5],NULL,marble_area_points,(void*)&parray[5]);
    if (ab != 0){
        yes[5] = false;
        printf("THREAD CREATE ERROR\n");
    }
    
    if (yes[0]) {
        ab = pthread_join(calcthreads[0],(void**)&result[0]);
        if (ab != 0){
            *result[0] = 0;
        }
    }
    if (yes[1]){ 
        ab = pthread_join(calcthreads[1],(void**)&result[1]);
        if (ab != 0){
            *result[1] = 0;
        }
    }
    if (yes[2]){ 
        ab = pthread_join(calcthreads[2],(void**)&result[2]);
        if (ab != 0){
            *result[2] = 0;
        }
    }    
    if (yes[3]){ 
        ab = pthread_join(calcthreads[3],(void**)&result[3]);
        if (ab != 0){
            *result[3] = 0;
        }
    }
    if (yes[4]){ 
        ab = pthread_join(calcthreads[4],(void**)&result[4]);
        if (ab != 0){
            *result[4] = 0;
        }
    }
    if (yes[5]){ 
        ab = pthread_join(calcthreads[5],(void**)&result[5]);
        if (ab != 0){
            *result[5] = 0;
        }
    }
    int* sum = (int*)malloc(1 * sizeof(int));
    *sum = *result[0] + *result[1] + *result[2] + *result[3] + *result[4] + *result[5];
    
    for(i = 0;i < calcfuncsize;i++){
        free(result[i]);
    }
    free(result);
    free(calcthreads);
    free(data);
    for (i = 0;i < calcfuncsize;i++){
        freedata2(parray[i]);
    }
    free(yes);
    return sum;
}

int which(int x, int y){
    int j;
    int i;
    
    for (i = 0;i < 17;i++){
        for (j = 1;j < 2 * frames[i][0];j += 2){
            if (frames[i][j] == x && frames[i][j+1] == y){
                return i;
            }
        }
    }
}
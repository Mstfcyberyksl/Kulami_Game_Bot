#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>


#define THREADSIZE 18
#define calcfuncsize 6

pthread_mutex_t mutexcalcrunning = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutexgeneral;
pthread_mutex_t filemutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t threads[THREADSIZE-calcfuncsize];
bool running[THREADSIZE-calcfuncsize];
int runningthreadsize = 0;
int area = 0, userframe = -1, pcframe = -1;
int genstep = -1;

int board2[8][8];
FILE* file ;

int marble_result,oneslen = 0;

int** ones;

bool** checked;

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

typedef struct {
    int color;
    int** board;
} Data2;

typedef struct Node {
    int frame;
}Node;
Node* newnode[8][8];
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
    for(n = 0;n < 28;n++){
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
    printf("REMOVE %d %d\n",x,y);
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
    printf("SUCCESS\n");
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
    Data2* parray = (Data2*)malloc(calcfuncsize * sizeof(Data2));
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
    for(i = 0;i < calcfuncsize;i++){
        if (yes[i]){
            free(parray[i].board);
        }
        freedata2(parray[i]);
    }
    free(parray);
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

void append(Data *data){
    pthread_mutex_lock(&filemutex);
    
    int i;
    for(i = 0;i < 33;i++){
        fprintf(file,"%d,",data->result[i]);
    }
    fprintf(file, "\n");
    pthread_mutex_unlock(&filemutex);
}
void* search(void *arg){
    printf("SEARCH FUNCTION\n");
    int i,j,k,length = 0,maximum = -1,info1,info2,a,b;
    Data* data = (Data*)arg;
    int x = data->x;
    int y = data->y;
    int step = data->step;
    printf("step = %d",step);
    int not_x = data->not_x;
    int not_y = data->not_y;
    int color = data->color;
    bool ret = data->ret;
    int** board = (int**)malloc(8 * sizeof(int*));
    int* result2 = (int*)malloc(33 * sizeof(int));
    int index = data->index;
    for(i = 0;i < 8;i++){
        board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(board[i],data->board[i],8 * sizeof(int));
    }
    memcpy(result2, data->result, 33 * sizeof(int));
    Data** datas = (Data**)malloc(28 * sizeof(Data*));
    bool* created = (bool*)malloc(28 * sizeof(bool));
    for(i = 0;i < 28;i++){
        created[i] = false;
    }
    int** array;
    int* result;
    int* invalid;
    invalid = (int*)malloc(1 * sizeof(int));
    result = (int*)malloc(2 * sizeof(int));
    array = (int**)malloc(1 * sizeof(int*));
    if (not_x > -1 && not_y > -1){
        info1 = newnode[not_x][not_y]->frame;
    }else{
        info1 = -1;
    }
    info2 = newnode[x][y]->frame;
    board[x][y] = color;
    if (step == 0){
        pthread_mutex_lock(&mutexcalcrunning);
        *invalid = *calculate(2,board);
        pthread_mutex_unlock(&mutexcalcrunning);
        free(result);
        data->result[0] = *invalid;
        append(data);
        pthread_mutex_lock(&mutexgeneral);
        running[data->index] = false;
        runningthreadsize--;
        pthread_mutex_unlock(&mutexgeneral);
        return (void*)invalid;
    }
    
    if (color == 2){
        color = 1;
    }
    else if (color == 1)
    {
        color = 2;
    }
    else{
        printf("COLOR ERROR %d",color);
    }
    int* used = (int*)malloc((THREADSIZE-calcfuncsize) * sizeof(int));
    int usedindex = 0;
    for (k = 0;k < 28;k++){
        if ((x + directions[k][0] != not_x || 
            y + directions[k][1] != not_y) && 
            x + directions[k][0] < 8 && 
            x + directions[k][0] > -1 && 
            y + directions[k][1] < 8 && 
            y + directions[k][1] > -1 && 
            board[x + directions[k][0]][y + directions[k][1]] == 0 && 
            newnode[x + directions[k][0]][y + directions[k][1]]->frame != info1 && 
            newnode[x + directions[k][0]][y + directions[k][1]]->frame != info2){
            
            length++;
            datas[k] = (Data*)malloc(sizeof(Data));
            board[x + directions[k][0]][y + directions[k][1]] = color;
            array = (int**)realloc(array,length * sizeof(int*));
            array[length-1] = (int*)malloc(3 * sizeof(int));
            datas[k]->x = x + directions[k][0];
            datas[k]->y = y + directions[k][1];
            datas[k]->step = step - 1;
            datas[k]->not_x = x;
            datas[k]->not_y = y;
            datas[k]->color = color;
            datas[k]->ret = false;
            datas[k]->board = (int**)malloc(8 * sizeof(int*));
            for (i = 0; i < 8; i++) {
                datas[k]->board[i] = (int*)malloc(8 * sizeof(int));
                memcpy(datas[k]->board[i], board[i], 8 * sizeof(int));
            }
            if ((2*genstep)-(2*step)+3 >= 33){
                printf("INDEX OUT OF BOUNDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD\n");
            }
            result2[(2*genstep)-(2*step)+3] = x + directions[k][0];
            result2[(2*genstep)-(2*step)+4] = y + directions[k][1];
            datas[k]->result = (int*)malloc(33 * sizeof(int));
            memcpy(datas[k]->result, result2, 33 * sizeof(int));
            pthread_mutex_lock(&mutexgeneral);
            
            if (runningthreadsize != THREADSIZE-calcfuncsize){
                pthread_mutex_unlock(&mutexgeneral);

                int c;
                for(c = 0 ; c < THREADSIZE-calcfuncsize;c++){
                    pthread_mutex_lock(&mutexgeneral);
                    if (!running[c]){
                        datas[k]->index = c;
                        used[usedindex] = c;
                        usedindex++;
                        running[c] = true;
                        runningthreadsize++;
                        pthread_mutex_unlock(&mutexgeneral);
                        int status = pthread_create(&threads[c],NULL,search,(void*)datas[k]);
                        if (status != 0){
                            printf("THREAD CREATE ERROR\n");
                        }
                        pthread_mutex_lock(&mutexgeneral);
                        break;
                    }
                    pthread_mutex_unlock(&mutexgeneral);
                }
            }else{
                pthread_mutex_unlock(&mutexgeneral);
                datas[k]->index = index;
                pthread_mutex_lock(&mutexgeneral);
                
                if (!running[index]){
                    pthread_mutex_unlock(&mutexgeneral);
                    used[usedindex] = index;
                    usedindex++;
                    pthread_mutex_lock(&mutexgeneral);
                    running[index] = true;
                    runningthreadsize++;
                    pthread_mutex_unlock(&mutexgeneral);
                    int status = pthread_create(&threads[index],NULL,search,(void*)datas[k]);
                    if (status != 0){
                        printf("THREAD CREATE ERROR\n");
                    }
                }else{
                    pthread_mutex_unlock(&mutexgeneral);
                    search((void*)datas[k]);
                }
            }
            pthread_mutex_unlock(&mutexgeneral);
            
            created[k] = true;
            array[length-1][1] = x + directions[k][0];
            array[length-1][2] = y + directions[k][1];
            board[x + directions[k][0]][y + directions[k][1]] = 0;
        }
    }
    int** results = (int**)malloc(28 * sizeof(int*));
    length = 0;
    if (ret){
        printf("USED INDEX = %d\n",usedindex);
    }
    for(k = 0;k < usedindex;k++){
            if (ret){
                printf("k = %d\n",k);
            }
            length++;
            results[k] = (int*)malloc(sizeof(int));
            void* thread_result;
            // bura harbi sıkıntı glb
            int resulta = pthread_join(threads[used[k]], &thread_result);
            if (resulta == 0){
                pthread_mutex_lock(&mutexgeneral);
                running[used[k]] = false;
                runningthreadsize--;
                pthread_mutex_unlock(&mutexgeneral);
                results[k] = (int*)thread_result;
                if (results[k] != NULL){
                    array[length-1][0] = *results[k];
                }else{
                    printf("NULLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLLL\n");
                }
            }else{
                array[length-1][0] = 0;
                printf("TRY JOIN ERROR\n");
            }
    }
    if (ret){
        printf("FINALLY\n");
    }
    for(i = 0;i < length;i++){
        if (array[i][0] > maximum){
            maximum = array[i][0];
            result[0] = array[i][1];
            result[1] = array[i][2];
            free(array[i]);
        }
    }
    free(array);
    pthread_mutex_lock(&mutexgeneral);
    running[data->index] = false;
    runningthreadsize--;
    pthread_mutex_unlock(&mutexgeneral);
    if (ret){
        return (void*)result;
    }
    *invalid = maximum;
    return (void*)invalid;
}

int* best_place(int x, int y,int step, int lx, int ly){
    int x_start,y_start;
    x_start = x;
    y_start = y;
    
    board2[x][y] = 1;
    int i,j;
    for(i = 0;i < THREADSIZE-calcfuncsize;i++){
        running[i] = false;
    }
    genstep = step;
    i = newnode[x][y]->frame;
    if (i != userframe && i != pcframe){
        userframe = i;
    }
    else{
        int* temp;
        temp = (int*)malloc(2 * sizeof(int));
        temp[0] = -1;
        temp[1] = -1;
        return temp;
    }

    file = fopen("data.txt","a");
    int* temp;
    Data data;
    data.x = x;
    data.y = y;
    data.step = step;
    data.not_x = lx;
    data.not_y = ly;
    data.color = 2;
    data.ret = true;
    int** board3 = (int**)malloc(8 * sizeof(int*));
    for (i = 0;i < 8;i++){
        board3[i] = (int*)malloc(8 * sizeof(int));
    }
    for (i = 0;i < 8;i++){
        for (j = 0;j < 8;j++){
            board3[i][j] = board2[i][j];
        }
    }
    data.board = board3;
    data.result = (int*)malloc(33 * sizeof(int));
    for (i = 3;i < 33;i++){
        data.result[i] = -1;
    }
    data.result[0] = -1;
    data.result[1] = x;
    data.result[2] = y;
    
    data.index = 0;
    running[0] = true;
    runningthreadsize++;
    temp = (int*)search((void*)&data);
    board2[temp[0]][temp[1]] = 2;

    j = newnode[temp[0]][temp[1]]->frame;
    if (j != userframe && j != pcframe){
        pcframe = j;
    }
    else{
        printf("PC FRAME ERROR\n");
        printf("PC FRAME = %d\n",pcframe);
        printf("USER FRAME = %d\n",userframe);
        printf("j = %d\n",j);
    }
    fclose(file);
    return temp;
}

int main(){
    int i,j;
    ones = (int**)malloc(1 * sizeof(int*));
    // make everywhere empty

    for (i = 0;i < 8; i++){
        for (j = 0;j < 8;j++){
            board2[i][j] = 0;
        }
    }
    // make checked array empty
    checked = (bool**)malloc(8 * sizeof(bool*));
    for(i = 0;i < 8;i++){
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

    return 0;
}
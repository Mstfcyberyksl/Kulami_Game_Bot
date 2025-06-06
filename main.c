#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define THREADSIZE 13
#define calcfuncsize 6 
#define rows 8
#define columns 8
#define directionsize 28
#define path_size 33

//debug.txt'ye bakarsak calculate function'unun öncesindeki printf kısmından 889 sonraki kısmınndan 890 tane var
// benzer şekilde tek threadin search'i recursive olarak çağırma kısmında öncesindeki kısmında sonrakindeki kısmına göre 
// 2 tane daha fazla var
pthread_mutex_t mutexcalcrunning = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t filemutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t finishcond = PTHREAD_COND_INITIALIZER;
bool finish = false;

int area = 0, userframe = -1, pcframe = -1;
int genstep = -1;

int finishsize1 = 0;
int finishsize2 = 0;

int board2[8][8];
FILE* file ;

int marble_result,oneslen = 0;

int** ones;

int** decisionboard;
int decisionboardlen = 0;

bool** checked;

int directions[directionsize][2] = {
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

void search(void *arg);

typedef struct {
    int color;
    int** board;
    void* (*func)(void*);
    int result;
    bool returned;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
}Data2;

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
    int* path;
    int* result;
    bool returned;
    pthread_cond_t cond;
    pthread_mutex_t mutex;
}Data;

// 0. index nedense executelenmiyo
// x tane head = %d iken döndü kısmı varsa x-1 tane returned gösteriyo
// üstte 2 tane CALCPOP'la başlayan statement olmasına rağmen 5 tane head = %d statementi var
// olay 99.99999% bu kısımla alakalı hele 3. yorum satırındaki olay kesin bişeyler diyo

typedef struct {
    void (*func)(void*);
    Data* data;
    bool exit;
    int index;
} Taskgeneral;

typedef struct {
    pthread_t threads[THREADSIZE];
    Taskgeneral* tasks;
    int* available;
    int index;
    
    int taskcount;
    bool exit;
    
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_cond_t cond_add;
    pthread_cond_t cond_give;
} Taskgeneralpool;
Taskgeneralpool generalpool;

Taskgeneral generalpop(){
    Taskgeneral task;
    pthread_mutex_lock(&generalpool.mutex);
    
    while (generalpool.taskcount == 0 && !generalpool.exit){
        printf("wait");
        pthread_cond_wait(&generalpool.cond_give, &generalpool.mutex);
    }

    if (generalpool.exit){
        printf("EXIT GENERALPOP\n");
        task.exit = true;
        pthread_mutex_unlock(&generalpool.mutex);
        return task;
    }
    if (generalpool.taskcount < 0){
        printf("ERROR: generalpool.taskcount < 0\n");
    }
    task = generalpool.tasks[generalpool.available[generalpool.index-1]];
    generalpool.available[generalpool.index-1] = -1;
    generalpool.index--;
    generalpool.taskcount--;
    
    printf("continue");
    pthread_cond_signal(&generalpool.cond_add);
    pthread_mutex_unlock(&generalpool.mutex);
    return task;
}

void* generalworkers(void* arg){
    while(1){
        Taskgeneral task = generalpop();
        if (task.exit){
            return NULL;
        }
            
        
        task.func(task.data);
        task.data->returned = true;
        
    }
}

void addgeneraltask(void* arg){
    pthread_mutex_lock(&generalpool.mutex);
    while(generalpool.taskcount == THREADSIZE){
        pthread_cond_wait(&generalpool.cond_add, &generalpool.mutex);
    }
    
    generalpool.available[generalpool.index] = generalpool.index;
    
    generalpool.tasks[generalpool.index].func = search;
    generalpool.tasks[generalpool.index].data = (Data*)arg;
    generalpool.tasks[generalpool.index].exit = false;
    generalpool.index++;
    generalpool.taskcount++;
    
    
    pthread_cond_signal(&generalpool.cond_give);
    pthread_mutex_unlock(&generalpool.mutex);
}

void print_board(int** board){
    for(int i = 0;i < rows;i++){
        for(int j = 0;j < columns;j++){
            printf("%d ",board[i][j]);
        }
        printf("\n");
    }
}

void* horizontal_points(void *arg){
    int length, pc = 0,user = 0;

    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        perror("Failed to allocate memory in horizontal_points function");
        pthread_exit(NULL);
    }
    Data2* data = (Data2*)arg;

    int** board = (int**)malloc(rows * sizeof(int*));
    for (int i = 0;i < rows;i++){
        board[i] = (int*)malloc(columns * sizeof(int));
        memcpy(board[i],data->board[i],columns * sizeof(int));
    }
    int color = data->color;

    for (int i = 0;i < rows;i++){
        length = 1;
        for (int j = 1;j < columns;j++){
            if (board[i][j] != 0 && board[i][j-1] == board[i][j]){
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
    }

    *result = pc - user;
    for (int i = 0;i < rows;i++){
        free(board[i]);
    }
    free(board);
    return (void*)result;
}
void* vertical_points(void *arg){
    int i,j,length,pc = 0,user = 0;

    Data2* data = (Data2*)arg;
    int** board = (int**)malloc(rows * sizeof(int*));
    for (i = 0;i < rows;i++){
        board[i] = (int*)malloc(columns * sizeof(int));
        memcpy(board[i],data->board[i],columns * sizeof(int));
    }

    int color = data->color;

    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        perror("Failed to allocate memory in vertical_points function");
        pthread_exit(NULL);
    }

    for (j = 0;j < rows;j++){
        length = 1;
        for (i = 1;i < columns;i++){
            if (board[i][j] != 0 && board[i-1][j] == board[i][j]){
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
    }

    *result = pc - user;
    for (i = 0;i < rows;i++){
        free(board[i]);
    }
    free(board);
    return (void*)result;
}

void* diagonal_points_45(void *arg){
    int length, pc = 0, user = 0, x, y;
    int places[7][2] = {
        {7,0},{6,0},{5,0},{4,0},{7,3},{7,2},{7,1}
    };
    Data2* data = (Data2*)arg;
    int** board = (int**)malloc(rows * sizeof(int*));
    for (int i = 0;i < rows;i++){
        board[i] = (int*)malloc(columns * sizeof(int));
        memcpy(board[i],data->board[i],columns * sizeof(int));
    }
    int color = data->color;

    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        printf("HEREEEEEEEEEEEEEEEEEEE 45\n");
        perror("Failed to allocate memory 45");
        pthread_exit(NULL);
    }

    for(int i = 0;i < 7;i++){
        x = places[i][0];
        y = places[i][1];
        length = 1;
        while (x < rows && y < columns && x > -1 && y > -1){
            if (x+1 < rows && x+1 > -1 && y-1 < columns && y-1 > -1 && board[x][y] != 0 && board[x][y] == board[x+1][y-1]){
                length++;
            }
            else{
                if (length >= 5){
                    if (x+1 < rows && x+1 > -1 && y-1 < columns && y-1 > -1 && board[x+1][y-1] == 2){
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
    for (int i = 0;i < rows;i++){
        free(board[i]);
    }
    free(board);
    return (void*)result;
}
void* diagonal_points_135(void *arg){
    int length, pc = 0, user = 0,x,y;
    int places[7][2] = {
        {7,4},{6,7},{5,7},{4,7},{7,5},{7,6},{7,7}
    };

    Data2* data = (Data2*)arg;
    int** board = (int**)malloc(rows * sizeof(int*));
    for (int i = 0;i < rows;i++){
        board[i] = (int*)malloc(columns * sizeof(int));
        memcpy(board[i],data->board[i],columns * sizeof(int));
    }
    int color = data->color;

    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        printf("HEREEEEEEEEEEEEEEEEEEE 135\n");
        perror("Failed to allocate memory 135");
        pthread_exit(NULL);
    }

    for(int i = 0;i < 7;i++){
        x = places[i][0];
        y = places[i][1];
        length = 0;
        while (x > -1 && y > -1){
            if (x+1 < rows && x+1 > -1 && y+1 < columns && y+1 > -1 && board[x][y] != 0 && board[x][y] == board[x+1][y+1]){
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
    for (int i = 0;i < rows;i++){
        free(board[i]);
    }
    free(board);
    return (void*)result;
}
int dfs(int i,int j,int** board,int color){
    int k,m,n,temp = 0;
    
    if (i < 0 || i == rows || j < 0 || j == columns || board[i][j] != color) {
        return 0;
    }
    board[i][j] = 0;
    return 1 + dfs(i+1,j,board,color) + dfs(i-1,j,board,color) + dfs(i,j+1,board,color) + dfs(i,j-1,board,color);
}
void* marble_area_points(void *arg){
    Data2* data = (Data2*)arg;
    int user = 0, pc = 0, temp;
    int** board = (int**)malloc(rows * sizeof(int*));
    for (int i = 0;i < rows;i++){
        board[i] = (int*)malloc(columns * sizeof(int));
        memcpy(board[i],data->board[i],columns * sizeof(int));
    }
    int color = data->color;

    int* result = (int*)malloc(sizeof(int));
    if (result == NULL) {
        perror("Failed to allocate memory marble area");
        pthread_exit(NULL);
    }

    for(int i = 0;i < rows;i++){
        for(int j = 0;j < columns;j++){
            if(board[i][j] == 1){
                temp = dfs(i,j,board,1);
                if (temp > user){
                    user = temp;
                }
            }else if (board[i][j] == 2){
                temp = dfs(i,j,board,2);
                if (temp > pc){
                    pc = temp;
                }
            }
        }
    }

    *result = pc - user;

    for (int i = 0;i < rows;i++){
        free(board[i]);
    }
    free(board);
    return (void*)result;
}
void* place_area_points(void *arg){
    int pc = 0,user = 0,length = 0;
    int* result = (int*)malloc(sizeof(int));
    Data2* data = (Data2*)arg;
    int** board = (int**)malloc(rows * sizeof(int*));
    for (int i = 0;i < rows;i++){
        board[i] = (int*)malloc(columns * sizeof(int));
        memcpy(board[i],data->board[i],columns * sizeof(int));
    }
    int color = data->color;

    if (result == NULL) {
        printf("HEREEEEEEEEEEEEEEEEEEE PLACE\n");
        perror("Failed to allocate memory place area");
        pthread_exit(NULL);
    }

    for (int i = 0;i < 17;i++){
        pc = 0;
        user = 0;
        for (int j = 1; j < 2 * frames[i][0]; j += 2){
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
    for (int i = 0;i < rows;i++){
        free(board[i]);
    }
    free(board);
    return (void*)result;
}
Data2 copydata2(Data2* data){
    Data2 copy;
    copy.color = data->color;
    copy.board = (int**)malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        copy.board[i] = (int*)malloc(columns * sizeof(int));
        memcpy(copy.board[i], data->board[i], columns * sizeof(int));
    }
    return copy;
}
void freedata2(Data2* data){
    for (int i = 0; i < rows; i++) {
        free(data->board[i]);
    }
    free(data->board);
}

void freedata(Data* data){
    for(int i = 0; i < rows; i++){
        free(data->board[i]);
    }
    free(data->board);
    free(data->path);
}

int* calculate(int color, int** board){
    Data2* data = (Data2*)malloc(sizeof(Data2));
    data->color = color;
    data->board = (int**)malloc(rows * sizeof(int*));
    for (int i = 0;i < rows;i++){
        data->board[i] = (int*)malloc(columns * sizeof(int));
        memcpy(data->board[i],board[i],columns * sizeof(int));
    }
    Data2* parray = (Data2*)malloc(calcfuncsize * sizeof(Data2));
    for(int i = 0;i < calcfuncsize;i++){
        parray[i] = copydata2(data);
    }
    int** temp;
    temp = (int**)malloc(calcfuncsize * sizeof(int*));
    for(int i = 0;i < calcfuncsize; i++){
        temp[i] = (int*)malloc(sizeof(int));
    }
    temp[0] = horizontal_points((void*)&parray[0]);
    temp[1] = vertical_points((void*)&parray[1]);
    temp[2] = diagonal_points_45((void*)&parray[2]);
    temp[3] = diagonal_points_135((void*)&parray[3]);
    temp[4] = place_area_points((void*)&parray[4]);
    temp[5] = marble_area_points((void*)&parray[5]);
    int* sum = (int*)malloc(sizeof(int));
    *sum = *(int*)temp[0] + *(int*)temp[1] + *(int*)temp[2] + *(int*)temp[3] + *(int*)temp[4] + *(int*)temp[5];
    for(int i = 0;i < calcfuncsize; i++){
        freedata2(&parray[i]);
        //free(&parray[i]);
        free(temp[i]);
    }
    free(parray);
    free(temp);
    freedata2(data);
    free(data);

    return sum;
}

int which(int x, int y){
    for (int i = 0;i < 17;i++){
        for (int j = 1;j < 2 * frames[i][0];j += 2){
            if (frames[i][j] == x && frames[i][j+1] == y){
                return i;
            }
        }
    }
    return -1;
}
void append(Data *data){
    decisionboard = (int**)realloc(decisionboard,(decisionboardlen+1) * sizeof(int*));
    decisionboard[decisionboardlen] = (int*)malloc(3 * sizeof(int));
    decisionboard[decisionboardlen][0] = data->path[0];
    decisionboard[decisionboardlen][1] = data->path[3];
    decisionboard[decisionboardlen][2] = data->path[4];
    decisionboardlen++;
    finishsize1--;
    pthread_cond_signal(&finishcond);
    
    /*file = fopen("data.txt","a");
    for(int i = 0;i < path_size;i++){
        fprintf(file,"%d,",data->path[i]);
    }
    fprintf(file, "\n");
    fclose(file);*/
}
void search(void *arg){
    printf("ha");
    finishsize2++;
    Data* data = (Data*)arg;
    if (data->step == 0){
        data->path[0] = *calculate(2,data->board);
        //append(data);
        
    }

    int length = 0, info1, info2, a, b;
    int* maximum = (int*)malloc(sizeof(int));
    Data** datas = (Data**)malloc(directionsize * sizeof(Data*));
    
    int* result;

    result = (int*)malloc(2 * sizeof(int));
    
    if (data->not_x > -1 && data->not_y > -1){
        info1 = newnode[data->not_x][data->not_y]->frame;
    }else{
        info1 = -1;
    }
    info2 = newnode[data->x][data->y]->frame;
    data->board[data->x][data->y] = data->color;

    if (data->color == 2){
        data->color = 1;
    }else if (data->color == 1){
        data->color = 2;
    }else{
        printf("COLOR ERROR %d",data->color);
    }
    for (int k = 0;k < directionsize;k++){
        if ((data->x + directions[k][0] != data->not_x ||
            data->y + directions[k][1] != data->not_y) &&
            data->x + directions[k][0] < rows &&
            data->x + directions[k][0] > -1 &&
            data->y + directions[k][1] < columns &&
            data->y + directions[k][1] > -1 &&
            data->board[data->x + directions[k][0]][data->y + directions[k][1]] == 0 &&
            newnode[data->x + directions[k][0]][data->y + directions[k][1]]->frame != info1 &&
            newnode[data->x + directions[k][0]][data->y + directions[k][1]]->frame != info2){

                length++;
                data->board[data->x + directions[k][0]][data->y + directions[k][1]] = data->color;
                
                datas[k] = (Data*)malloc(sizeof(Data));
                datas[k]->x = data->x + directions[k][0];
                datas[k]->y = data->y + directions[k][1];
                datas[k]->step = data->step - 1;
                if (data->step - 1 == 0){
                    finishsize1++;
                }
                datas[k]->not_x = data->x;
                datas[k]->not_y = data->y;
                datas[k]->color = data->color;
                datas[k]->ret = false;
                datas[k]->board = (int**)malloc(rows * sizeof(int*));
                for (int i = 0; i < rows; i++) {
                    datas[k]->board[i] = (int*)malloc(columns * sizeof(int));
                    memcpy(datas[k]->board[i], data->board[i], columns * sizeof(int));
                }

                data->path[(2*genstep)-(2*data->step)+3] = data->x + directions[k][0];
                data->path[(2*genstep)-(2*data->step)+4] = data->y + directions[k][1];
                datas[k]->path = (int*)malloc(path_size * sizeof(int));
                memcpy(datas[k]->path, data->path, path_size * sizeof(int));
                addgeneraltask((void*)datas[k]);
                //freedata(datas[k]);
                free(datas[k]);

                data->board[data->x + directions[k][0]][data->y + directions[k][1]] = 0;
        }
    }
    
    for(int i = 0; i < rows; i++){
        free(data->board[i]);
    }
    free(data->board);
    free(data->path);
    free(datas);

    //freedata(data);
    //free(data);
    finishsize2--;
    pthread_cond_signal(&finishcond);
    
}

int* best_place(int x, int y,int step, int lx, int ly){
    int i,j;
    
    // neden calcpoolun tasklerinin datalarına yer vermişim de generalpoolun vermemişim
    // ve sorun bundan kaynaklı olabilir mi?
    
    generalpool.taskcount = 0;
    generalpool.tasks = (Taskgeneral*)malloc((THREADSIZE) * sizeof(Taskgeneral));
    generalpool.available = (int*)malloc(THREADSIZE * sizeof(int));
    generalpool.index = 0;

    for(i = 0;i < THREADSIZE;i++){
        generalpool.tasks[i].data = (Data*)malloc(sizeof(Data));
        generalpool.tasks[i].data->returned = false;
        generalpool.tasks[i].exit = false;
        generalpool.available[i] = -1;
        
    }
    generalpool.exit = false;
    

    for(i = 0;i < THREADSIZE;i++){
        pthread_create(&generalpool.threads[i],NULL,generalworkers,&generalpool);
    }
    
    board2[x][y] = 1;
    
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

    
    int* temp;
    Data data;
    data.x = x;
    data.y = y;
    data.step = step;
    data.not_x = lx;
    data.not_y = ly;
    data.color = 2;
    data.ret = true;
    *data.result = -1;
    data.returned = false;

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
    data.path = (int*)malloc(33 * sizeof(int));
    for (i = 3;i < 33;i++){
        data.path[i] = -1;
    }
    data.path[0] = -1;
    data.path[1] = x;
    data.path[2] = y;
    
    addgeneraltask((void*)&data);
    pthread_mutex_lock(&generalpool.mutex);
    while (finishsize1 > 0 && finishsize2 > 0){
        pthread_cond_wait(&finishcond, &generalpool.mutex);
    }
    pthread_mutex_unlock(&generalpool.mutex);
    printf("KEEPS GOING\n");
    temp = (int*)malloc(2 * sizeof(int));
    temp[0] = 0;
    temp[1] = 0;
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
    
    pthread_mutex_lock(&generalpool.mutex);
    generalpool.exit = true;
    pthread_cond_broadcast(&generalpool.cond);
    pthread_mutex_unlock(&generalpool.mutex);

    for(i = 0;i < THREADSIZE;i++){
        pthread_join(generalpool.threads[i],NULL);
        //free(generalpool.tasks[i].data);
    }  
    printf("GENERAL ARE JOINED\n");
    return temp;
}

int main(){
    int i,j;
    ones = (int**)malloc(1 * sizeof(int*));
    // make everywhere empty

    decisionboard = (int**)malloc(sizeof(int*));
    
    pthread_mutex_init(&generalpool.mutex,NULL);
    pthread_cond_init(&generalpool.cond,NULL);

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
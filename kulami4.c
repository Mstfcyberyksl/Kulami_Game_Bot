#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
//#define THREADSIZE 20

//pthread_t* threads[THREADSIZE];
//bool running[THREADSIZE];
// ones array //
// append function //
//printf statements
//puanlar(sum ve result variableları) fazla çıkıyo //board full olma ihtimali belki biraz var ama bence fonklarda sıkıntı var
// çnk hepsi belli bi renk olsa bile 160ları görmez bence
// marble area fonksiyonunun race conditionuna ve 
// fonksiyonlarun döndürdüğü değerlere dikkat et (boardı yazdır manuel kontrol et)
int marble_result, oneslen = 0, area = 0, userframe = -1, pcframe = -1;
int whichcount = 0,calculatecount = 0,searchcount = 0;
int genstep = -1;
int** ones;
bool** checked;
int board2[8][8];
FILE* file ;
typedef struct {
    int color;
    int** board;
} Data2;

typedef struct Node {
    int frame;
}Node;
Node* newnode[8][8];
int directions[28][2] = {
    {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {0, 6}, {0, 7},
    {0, -1}, {0, -2}, {0, -3}, {0, -4}, {0, -5}, {0, -6}, {0, -7},
    {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0},
    {-1, 0}, {-2, 0}, {-3, 0}, {-4, 0}, {-5, 0}, {-6, 0}, {-7, 0}
};
pthread_t threads[5];
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

void* horizontal_points(void *arg){
    int i,j,length = 0,pc = 0,user = 0;

    int* result = malloc(sizeof(int));
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
    //memcpy(board,data->board,sizeof(board));

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
    /*printf("board = \n");
    for(i = 0;i < 8;i++){
        for(j = 0;j < 8;j++){
            printf("%d ",board[i][j]);
        }
        printf("\n");
    }
    printf("pc = %d user = %d result = %d\n",pc,user,*result);*/
    printf("horizontal = %d\n",*result);
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

    int* result = malloc(sizeof(int));
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
    printf("vertical = %d\n",*result);
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

    int* result = malloc(sizeof(int));
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
    printf("diagonal 45 = %d\n",*result);
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

    int* result = malloc(sizeof(int));
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
    printf("diagonal 135 = %d\n",*result);
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

    int* result = malloc(sizeof(int));
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
                ones[oneslen-1] = (int*)malloc(2 * sizeof(int)); // ztn var olan bi alana malloc atamadan gide3bilir
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
    printf("marble area = %d\n",*result);
    pthread_mutex_unlock(&mutex);
    return (void*)result;   
    
    
}
    
    
void* place_area_points(void *arg){
    int i,j,pc = 0,user = 0,length = 0;
    int* result = malloc(sizeof(int));

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
    printf("place area = %d\n",*result);
    return (void*)result;
}


int* calculate(int color, int** board){
    // don't forget to free the results

    calculatecount++;
    int i;
    int** result = (int**)malloc(5 * sizeof(int*));
    for (i = 0;i < 5;i++){
        result[i] = (int*)malloc(1 * sizeof(int));
    }
    pthread_t* threads2 = (pthread_t*)malloc(6 * sizeof(pthread_t));
    Data2* data3 = (Data2*)malloc(sizeof(Data2));
    data3->color = color;
    data3->board = (int**)malloc(8 * sizeof(int*));
    for (int i = 0;i < 8;i++){
        data3->board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(data3->board[i],board[i],8 * sizeof(int));
    }
    
    pthread_create(&threads2[0],NULL,horizontal_points,(void*)data3);
    pthread_create(&threads2[1],NULL,vertical_points,(void*)data3);
    pthread_create(&threads2[2],NULL,diagonal_points_45,(void*)data3);
    pthread_create(&threads2[3],NULL,diagonal_points_135,(void*)data3);
    pthread_create(&threads2[4],NULL,place_area_points,(void*)data3);
    pthread_create(&threads2[5],NULL,marble_area_points,(void*)data3);
    
    pthread_join(threads2[0],(void**)&result[0]);
    pthread_join(threads2[1],(void**)&result[1]);
    pthread_join(threads2[2],(void**)&result[2]);
    pthread_join(threads2[3],(void**)&result[3]);
    pthread_join(threads2[4],(void**)&result[4]);
    pthread_join(threads2[5],(void**)&result[5]);

    int* sum = (int*)malloc(1 * sizeof(int));
    *sum = *(int*)result[0] + *(int*)result[1] + *(int*)result[2] + *(int*)result[3] + *(int*)result[4] + *(int*)result[5];
    for (i = 0;i < 6;i++){
        free(result[i]);
    }
    free(result);
    free(data3);
    return sum;
    
    
}


int which(int x, int y){
    whichcount++;
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
// buraya dikkat et 15 olunca 33'ten daha büyük olması gerekebilir 13 için başka 15 için başka hata oldu gibi
    // daha küçük bir sayı olduğunda geri kalanını -1 yap
    // aborted hatasını araştır ve bak
    // 15 için aborted hatası verene kdr ilk buna uğraş sonra aborteda geç
    // 1 satırdan sonra segmentation fault verdi bu bir işaret
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
}Data;

void append(Data *data){
    
    
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    
    int i;
    for(i = 0;i < 33;i++){
        fprintf(file,"%d,",data->result[i]);
    }
    fprintf(file, "\n");
    pthread_mutex_unlock(&mutex);
    
}

void* search(void *arg){
    
    int i,j,k,length = 0,maximum = -1,info1,info2,a,b;
    Data* data = (Data*)arg;
    int x = data->x;
    int y = data->y;
    int step = data->step;
    int not_x = data->not_x;
    int not_y = data->not_y;
    int color = data->color;
    bool ret = data->ret;
    int** board ;
    int* result2;
    board = data->board;
    result2 = data->result;
    
    Data** datas = (Data**)malloc(28 * sizeof(Data*));
    pthread_t* threads = (pthread_t*)malloc(28 * sizeof(pthread_t));
    bool* created = (bool*)malloc(28 * sizeof(bool));
    for(i = 0;i < 28;i++){
        created[i] = false;
    }
    int** array;
    int* result;
    int* invalid;
    searchcount++;
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
        pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
        pthread_mutex_lock(&mutex);
        
        pthread_mutex_unlock(&mutex);
        *invalid = *calculate(2,board);
        
        free(result);
        data->result[0] = *invalid;
        
        append(data);
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
            pthread_create(&threads[k],NULL,search,(void*)datas[k]);
            created[k] = true;
            array[length-1][1] = x + directions[k][0];
            array[length-1][2] = y + directions[k][1];
            board[x + directions[k][0]][y + directions[k][1]] = 0;
        }
    }
    int** results = (int**)malloc(28 * sizeof(int*));
    length = 0;
    for(k = 0;k < 28;k++){
        if (created[k]){
            length++;
            
            results[k] = (int*)malloc(sizeof(int));
            void* thread_result;
            int resulta = pthread_join(threads[k], &thread_result);
            if (resulta == 0){
                results[k] = (int*)thread_result;
                if (results[k] != NULL){
                    array[length-1][0] = *results[k];
                }
            }else{
                printf("TRY JOIN ERROR\n");
            }
            //pthread_join(threads[k], &thread_result);
            //results[k] = (int*)thread_result;
            if (results[k] != NULL){
                array[length-1][0] = *results[k];
            }else{
                printf("NULLLLLLLLLLLLLLLLLLLLLLLLL\n");
            }
            
        }  
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
    if (ret){
        return (void*)result;
    }
    *invalid = maximum;
    
    return (void*)invalid;
}

int* best_place(int x, int y,int step, int lx, int ly){
    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n");
    file = fopen("data.txt","a");
    int x_start,y_start;
    x_start = x;
    y_start = y;
    
    board2[x][y] = 1;
    int i,j;
    //for(i = 0;i < THREADSIZE;i++){
    //    running[i] = false;
    //}
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
    
    data.result[1] = x;
    data.result[2] = y;
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
    
   
    return temp;

    fclose(file);

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
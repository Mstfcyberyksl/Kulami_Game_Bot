#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
//#include "constants.h"
#include "calculations.h"

// marble area points functionunun dfs fonk kısmındaki check kısmındaki logicini kontrol et yeni bi 
// fonksiyonda tekrar sıfırlamayı gözden geçir


//  PRAGMA ONCE 
// HEADER FİLE'DA BİRDEN FAZLA EKLENEMEME AMA EKLEME OLAYI
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
    // local variables
    int i,j,k,length = 0,maximum = -1,frameinfoprev,frameinfocurrent,a,b,usedindex = 0,c;
    int* used = (int*)malloc((THREADSIZE-calcfuncsize) * sizeof(int));
    int** results = (int**)malloc(directionsize * sizeof(int*));
    Data** datas = (Data**)malloc(directionsize * sizeof(Data*));
    bool* created = (bool*)malloc(directionsize * sizeof(bool));
    for(i = 0;i < directionsize;i++){
        created[i] = false;
    }
    int** array;
    array = (int**)malloc(1 * sizeof(int*));
    int* result;
    result = (int*)malloc(2 * sizeof(int));
    int* invalid;
    invalid = (int*)malloc(1 * sizeof(int));

    // copy datas
    Data* data = (Data*)arg;
    int x = data->x;
    int y = data->y;
    int step = data->step;
    printf("step = %d",step);
    int not_x = data->not_x;
    int not_y = data->not_y;
    int color = data->color;
    bool ret = data->ret;
    int index = data->index;
    int** board = (int**)malloc(8 * sizeof(int*));
    for(i = 0;i < 8;i++){
        board[i] = (int*)malloc(8 * sizeof(int));
        memcpy(board[i],data->board[i],8 * sizeof(int));
    }
    int* result2 = (int*)malloc(33 * sizeof(int));
    memcpy(result2, data->result, 33 * sizeof(int));
    // end of copying

    
    if (not_x > -1 && not_y > -1){
        frameinfoprev = newnode[not_x][not_y]->frame;
    }else{
        frameinfoprev = -1;
    }
    frameinfocurrent = newnode[x][y]->frame;
    board[x][y] = color;

    // base case
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
    // end of base case
    
    // change colors
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
    // end of changing colors

    // creating new threads
    for (k = 0;k < directionsize;k++){
        if ((x + directions[k][0] != not_x || 
            y + directions[k][1] != not_y) && 
            x + directions[k][0] < 8 && 
            x + directions[k][0] > -1 && 
            y + directions[k][1] < 8 && 
            y + directions[k][1] > -1 && 
            board[x + directions[k][0]][y + directions[k][1]] == 0 && 
            newnode[x + directions[k][0]][y + directions[k][1]]->frame != frameinfoprev && 
            newnode[x + directions[k][0]][y + directions[k][1]]->frame != frameinfocurrent){
            
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
    free(used);
    free(created);
    free(result);
    free(invalid);
    free(data);
    free(result2);
    for (i = 0;i < 8;i++){
        free(board[i]);
    }
    free(board);
    for(i = 0;i < length;i++){
        free(results[i]);
    }
    free(results);
    for (i = 0;i < directionsize;i++){
        if (created[i]){
            free(datas[i]->board);
            free(datas[i]->result);
            free(datas[i]);
            // burda double free olabilir
        }
    }
    
    free(datas);
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

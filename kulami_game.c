#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
// 0 = empty, 1 = red , 2 = black
int** decision_array;
int** changed;
int** temp;
int* result;
int i,i2,j,m,n,length,length_of_decision_array,max_index,point,point_result,red_count,black_count,max_red_count,max_black_count;
int before_color,length_pos,length_neg,k,x_start,y_start,h;
int directions[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
bool check;
struct Node{
    int color;
    struct Node* up;
    struct Node* down;
    struct Node* left;
    struct Node* right;
    struct Node* topright;
    struct Node* topleft;
    struct Node* bottomleft;
    struct Node* bottomright;

};

struct Node* create_node(int color,int x,int y){
    struct Node* newnode = (struct Node*)malloc(sizeof(struct Node));

    newnode->color = color;
    newnode->up = NULL;
    newnode->down = NULL;
    newnode->left = NULL;
    newnode->right = NULL;
    newnode->topright = NULL;
    newnode->topleft = NULL;
    newnode->bottomleft = NULL;
    newnode->bottomright= NULL;

    return newnode;
}

struct Node* board[8][8];

bool is_valid(){

}
void return_start(int length){
    printf("RETURN_START FUNCTION STARTED \n");
    for (i = 1 ; i < length; i++){
        board[changed[i][0]][changed[i][1]]->color = 0;
    }

    i2 = 2;
    printf("RETURN_START FUNCTION ENDED \n");
}
int horizontal_points(int color){
    printf("HORIZONTAL_POINTS FUNCTION STARTED \n");
    point_result = 0;
    for(i = 0; i < 8; i++){
        point_result += length;
        j = 0;
        length = 0;
        while(j < 8){   // <----- OPTIMIZE HERE
            if(board[i][j]->color == color){
                if(board[i][j-1]->color == color || board[i][j-1]->color == 0){
                    length++;
                }
                else if (length < 5){
                    length = 0;
                }
            }
            else if(board[i][j]->color == 0 && (length < 5 && length > -5)){
                length = 0;
            }
            else{
                if(board[i][j-1]->color != color){
                    length--;
                }
                else if (length > -5){
                    length = 0;
                }
            }
            j++;
        }
    }
    return point_result;
}
int vertical_points(int color){
    printf("VERTICAL_POINTS FUNCTION STARTED \n");
    point_result = 0;
    for(j = 0; j < 8; j++){
        point_result += length;
        i = 0;
        length = 0;
        while(i < 8){    // <----- OPTIMIZE HERE
            if(board[i][j]->color == color){
                if(board[i-1][j]->color == color || board[i-1][j]->color == 0){
                    length++;
                }
                else if (length < 5){
                    length = 0;
                }
            }
            else if(board[i][j]->color == 0 && (length < 5 && length > -5)){
                length = 0;
            }
            else{
                if(board[i-1][j]->color != color){
                    length--;
                }
                else if (length > -5){
                    length = 0;
                }
            }
            i++;
        }
    }
    return point_result;
}
int diagonal_points_45(int color){
    printf("DIAGONAL_POINTS_45 FUNCTION STARTED \n");
    point_result = 0;

    for(m = 7; m > 3; m--){
        length_pos = 0;
        length_neg = 0;
        point_result += length_pos-length_neg;
        i = m;
        j = 0;

        for(j = 0;j < 7;j++){    // <----- OPTIMIZE HERE
            k = j;
            if(board[i][k]->color == color){
                if(board[i-1][k+1]->color == color){
                    length_pos++;
                }
                else if (length_neg > -5){
                    length_neg = 0;
                }
                i--;
                k++;
            }
            else if(board[i][k]->color == 0){
                if (length_pos < 5){
                    length_pos = 0;
                }
                if (length_neg > -5){
                    length_neg = 0;
                }
                k++;
            }
            else{
                if(board[i-1][k+1]->color != color){
                    length_neg--;
                }
                else if (length_pos < 5){
                    length_pos = 0;
                }
                i--;
                k++;
            }
        }
        if (length_pos > 5){
            length_pos = 0;
        }
        if (length_neg < -5){
            length_neg = 0;
        }
    }
    return point_result;
}
int diagonal_points_135(int color){
    printf("DIAGONAL_POINTS_135 FUNCTION STARTED \n");
    point_result = 0;

    for(m = 7; m > 3; m--){
        length_pos = 0;
        length_neg = 0;
        point_result += length_pos-length_neg;
        i = m;
        j = 0;

        for(j = 7;j > 0;j--){    // <----- OPTIMIZE HERE
            k = j;
            if(board[i][k]->color == color){
                if(board[i-1][k-1]->color == color){
                    length_pos++;
                }
                else if (length_neg > -5){
                    length_neg = 0;
                }
                i--;
                k--;
            }
            else if(board[i][k]->color == 0){
                if (length_pos < 5){
                    length_pos = 0;
                }
                if (length_neg > -5){
                    length_neg = 0;
                }
                k--;
            }
            else{
                if(board[i-1][k-1]->color != color){
                    length_neg--;
                }
                else if (length_pos < 5){
                    length_pos = 0;
                }
                i--;
                k--;
            }

        }
        if (length_pos > 5){
            length_pos = 0;
        }
        if (length_neg < -5){
            length_neg = 0;
        }
    }
    return point_result;
}

int marble_area_points(int color){
    printf("MARBLE_AREA_POINTSFUNCTION STARTED\n");
    
    return 0;
}
int place_area_points(int color){
    printf("PLACE_AREA_POINTS FUNCTION STARTED\n");
    return 0;
}
void check_point(int color, int x,int y,int length){
    printf("CHECK_POINT FUNCTION  STARTED \n");
    
    decision_array = (int**)realloc(decision_array, n * sizeof(int*));
    decision_array[n-1] = (int*)malloc(3 * sizeof(int));
    length_of_decision_array = n;
    decision_array[n-1][0] = x;
    decision_array[n-1][1] = y;
    point = horizontal_points(color) + vertical_points(color) + diagonal_points_45(color) + diagonal_points_135(color) + marble_area_points(color) + place_area_points(color);
    decision_array[n-1][2] = point;

    n++;
    return_start(length);
}
bool ischanged(int** changed,int x,int y){
    for(i = 1; i < i2; i++){
        if(changed[i][0] == x && changed[i][1] == y){
            return false;
        }
    }
    return true;
}
void search(int color, int x,  int y, int step,int x_start,int y_start){
    printf("SEARCH FUNCTION \n");
    if(step == 0){
        
        check_point(color,x_start,y_start,i2);
    }

    
    else{
    step--;
    for (i = 0;i < 4;i++){
        
        if (step == 3){
        x_start = x;
        y_start = y;
        }
        h = x + directions[i][0];
        j = y + directions[i][1];
        if(0 <= h && h < 8 && 0 <= j && j < 8 && board[h][j]->color == 0 && ischanged(changed, h,j)){
            changed = (int**)realloc(changed, i2+10 * sizeof(int*));
            if(changed == NULL){
                printf("Memory reallocation failed");
                changed = (int**)malloc(i2+1 * sizeof(int*));
            }
            changed[i2] = (int*)malloc( 2 * sizeof(int));
            changed[i2][0] = i ;
            changed[i2][1] = j ;
            i2++;
            
            search(color,h,j,step,x_start,y_start);
            
        }

    }
    }
}
int* best_place(int color,int x,int y,int step){
    printf("BEST_PLACE FUNCTION STARTED \n");
    x_start = x;
    y_start = y;
    if(color == 1){
        board[x][y]->color = 1;
        color++;
    }
    else{
        board[x][y]->color = 2;
        color--;
    }
    i2 = 1;
    n = 1;

    search(color,x,y,step,x_start,y_start); 
    max_index = 0;
    printf("length_of_decision_array = %d\n",length_of_decision_array);
    for(i = 1; i < length_of_decision_array;i++){
        
        if(decision_array[i][2] > decision_array[max_index][2]){
            max_index = i;
        }
    }
    result[0] = decision_array[max_index][0];
    result[1] = decision_array[max_index][1];
    decision_array = (int**)realloc(decision_array, 1 * sizeof(int*));
    return result;
}
int main(){
    printf("MAIN FUNC STARTED\n");
    int i1,j1;
    decision_array = (int**)malloc(1 * sizeof(int*));
   

    changed = (int**)malloc(1 * sizeof(int*));
    changed[0] = (int*)malloc(2 * sizeof(int));

    result = (int*)malloc(2 * sizeof(int));

    for (i1 = 0;i1 < 8;i1++){
        for(j1 = 0;j1 < 8;j1++){
            board[i1][j1] = create_node(0,i1,j1);
        }
    }

    for (i1 = 0;i1 < 8;i1++){
        for(j1 = 0;j1 < 8;j1++){
            if(i1 == 0){
                if(j1 == 0){
                board[i1][j1]->down = board[i1+1][j1];
                board[i1][j1]->right = board[i1][j1+1];
                board[i1][j1]->bottomright = board[i1+1][j1+1];
                }
                else if (j1 == 7){
                board[i1][j1]->down = board[i1+1][j1];
                board[i1][j1]->left = board[i1][j1-1];
                board[i1][j1]->bottomleft = board[i1+1][j1-1];
                }
                else{
                board[i1][j1]->down = board[i1+1][j1];
                board[i1][j1]->left = board[i1][j1-1];
                board[i1][j1]->right = board[i1][j1+1];
                board[i1][j1]->bottomleft = board[i1+1][j1-1];
                board[i1][j1]->bottomright = board[i1+1][j1+1];
                }
            }
            else if(i1 == 7){
                if(j1 == 0){
                    board[i1][j1]->up = board[i1-1][j1];
                board[i1][j1]->right = board[i1][j1+1];
                board[i1][j1]->topright = board[i1-1][j1+1];
                }
                else if (j1 == 7){
                    board[i1][j1]->up = board[i1-1][j1];
                board[i1][j1]->left = board[i1][j1-1];
                board[i1][j1]->topleft = board[i1-1][j1-1];
                }
                else{
                    board[i1][j1]->up = board[i1-1][j1];
                board[i1][j1]->left = board[i1][j1-1];
                board[i1][j1]->right = board[i1][j1+1];
                board[i1][j1]->topleft = board[i1-1][j1-1];
                board[i1][j1]->topright = board[i1-1][j1+1];
                }
            }
            else if(j1 == 0 && (i1 != 0 || i1 != 7)){
                board[i1][j1]->up = board[i1-1][j1];
                board[i1][j1]->down = board[i1+1][j1];
                board[i1][j1]->right = board[i1][j1+1];
                board[i1][j1]->topright = board[i1-1][j1+1];
                board[i1][j1]->bottomright = board[i1+1][j1+1];

            }
            else if(j1 == 7 && (i1 != 0 || i1 != 7)){
                board[i1][j1]->up = board[i1-1][j1];
                board[i1][j1]->down = board[i1+1][j1];
                board[i1][j1]->left = board[i1][j1-1];
                board[i1][j1]->topleft = board[i1-1][j1-1];
                board[i1][j1]->bottomleft = board[i1+1][j1-1];
            }
            else{
                board[i1][j1]->up = board[i1-1][j1];
                board[i1][j1]->down = board[i1+1][j1];
                board[i1][j1]->left = board[i1][j1-1];
                board[i1][j1]->right = board[i1][j1+1];
                board[i1][j1]->topleft = board[i1-1][j1-1];
                board[i1][j1]->topright = board[i1-1][j1+1];
                board[i1][j1]->bottomleft = board[i1+1][j1-1];
                board[i1][j1]->bottomright = board[i1+1][j1+1];
            }
        }
    }

    return 0;
}

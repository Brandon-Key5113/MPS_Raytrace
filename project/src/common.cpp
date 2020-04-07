#include "common.h"

int calcIndex(ConfigData* data,int row,int col){
    return 3 * ( row * data->width + col );
}

int calcIndexI(ConfigData* data,int row,int col){
    return 3 * ( col * data->height + row );
}


int isPerfectSquare(int n)
{
    int square = 1;
    int i = 1;
    while (square <= n){
        square = i*i;
        if (square == n){
            return i;
        }
        ++i;
    }
    return 0;
}

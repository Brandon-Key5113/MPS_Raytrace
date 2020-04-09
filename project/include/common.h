#ifndef __COMMON_H__
#define __COMMON_H__

#include "RayTrace.h"

typedef enum{
    MPI_MESSAGE_TAG_PIX = 8,
    MPI_MESSAGE_TAG_COMP_T
} MpiMesssageTag;


typedef struct BlockInfo{
    int sqrtProc;
    int colsMax;
    int rowsMax;

    int slave;

    int rowsNorm;
    int rowsExtra;
    int rowsRemain;
    int rowsRemainStart;
    int colsNorm;
    int colsExtra;
    int colsRemain;
    int colsRemainStart;

    int blockRow;
    int blockCol;

    int rowStart, rowEnd, rowsToCalc;
    int colStart, colEnd, colsToCalc;

    BlockInfo(ConfigData* data);

    void UpdateData(int slave);

} BlockInfo;

//This function is a helper function to calculate an index into a pixel array
//
//Inputs:
//    data - the ConfigData that holds the scene information.
//    row - the pixel row
//    col - the pixel column
//
//Outputs: index
int calcIndex(ConfigData* data,int row,int col);
//Column major index calc
int calcIndexI(ConfigData* data,int row,int col);

//Math function to check if a number is a perfect square
//
//Inputs:
//    n - Number to check
//
//Outputs:
//    0 - Number is not a perfect square
//    other - square root of the number
int isPerfectSquare(int n);


#endif

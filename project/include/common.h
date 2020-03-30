#ifndef __COMMON_H__
#define __COMMON_H__

#include "RayTrace.h"

typedef enum{
    MPI_MESSAGE_TAG_PIX = 8,
    MPI_MESSAGE_TAG_COMP_T
} MpiMesssageTag;

//This function is a helper function to calculate an index into a pixel array
//
//Inputs:
//    data - the ConfigData that holds the scene information.
//    row - the pixel row
//    col - the pixel column
//
//Outputs: index
int calcIndex(ConfigData* data,int row,int col);

#endif

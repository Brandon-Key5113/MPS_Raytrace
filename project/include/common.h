#ifndef __COMMON_H__
#define __COMMON_H__

#include "RayTrace.h"

typedef enum{
    MPI_MESSAGE_TAG_PIX = 8,
    MPI_MESSAGE_TAG_COMP_T
} MpiMesssageTag;


int calcIndex(ConfigData* data,int row,int col);

#endif

//This file contains the code that the master process will execute.

#include <iostream>
#include <mpi.h>
#include "RayTrace.h"
#include "slave.h"

void slaveMain(ConfigData* data)
{


    //Depending on the partitioning scheme, different things will happen.
    //You should have a different function for each of the required
    //schemes that returns some values that you need to handle.
    switch (data->partitioningMode)
    {
        case PART_MODE_NONE:
            //The slave will do nothing since this means sequential operation.
            break;
        case PART_MODE_STATIC_STRIPS_HORIZONTAL:

            break;
        case PART_MODE_STATIC_STRIPS_VERTICAL:

            break;

        case PART_MODE_STATIC_BLOCKS:

            break;
        case PART_MODE_STATIC_CYCLES_HORIZONTAL:

            break;
        case PART_MODE_STATIC_CYCLES_VERTICAL:

            break;
        case PART_MODE_DYNAMIC:

            break;
        default:
            std::cout << "This mode (" << data->partitioningMode;
            std::cout << ") is not currently implemented." << std::endl;
            break;
    }
}

void slaveStaticStripsHorizontal(ConfigData* data){

}

void slaveStaticStripsVertical(ConfigData* data){

}

void slaveStaticBlocks(ConfigData* data){

}

void slaveStaticCyclesHorizontal(ConfigData* data){

}

void slaveStaticCyclesVertical(ConfigData* data){

}

void slaveDynamic(ConfigData* data){

}

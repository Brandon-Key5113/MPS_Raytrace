//This file contains the code that the master process will execute.

#include <iostream>
#include <mpi.h>
#include "RayTrace.h"
#include "slave.h"
#include "common.h"

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
            slaveStaticStripsHorizontal(data, pixels);
            break;
        case PART_MODE_STATIC_STRIPS_VERTICAL:
            slaveStaticStripsVertical(data, pixels);
            break;
        case PART_MODE_STATIC_BLOCKS:
            slaveStaticBlocks(data, pixels);
            break;
        case PART_MODE_STATIC_CYCLES_HORIZONTAL:
            slaveStaticCyclesHorizontal(data, pixels);
            break;
        case PART_MODE_STATIC_CYCLES_VERTICAL:
            slaveStaticCyclesVertical(data, pixels);
            break;
        case PART_MODE_DYNAMIC:
            slaveDynamic(data, pixels);
            break;
        default:
            std::cout << "This mode (" << data->partitioningMode;
            std::cout << ") is not currently implemented." << std::endl;
            break;
    }
}

void slaveStaticStripsHorizontal(ConfigData* data, float* pixels){
    //Start the computation time timer.
    double computationStart = MPI_Wtime();

    // Vars to improve readability
    int colsMax = data->width;
    int rowsMax = data->height;
    // Number of rows to calcualte
    // Divide rows evenenly amoung processors
    int rowsPerProcNom = rowsMax/data->mpi_procs;
    // Some slaves needs to do addtitional work for the remainder
    int rowsPerProcExtra = rowsPerProcNom + 1;
    // Int maths leaves a remainder. The remainder will later be distributed to
    // the first n slaves.
    int rowsRemain = rowsMax%data->mpi_procs;

    // How many rows each slaves calcs
    int rowsToCalc;
    // Where in the array this slave starts
    int rowsStart;
    if (data->mpi_rank < rowsRemain){
        rowsToCalc = rowsPerProcExtra;
        rowsStart = (rowsPerProcExtra* data->mpi_rank);
    } else {
        rowsToCalc = rowsPerProcNom;
        rowsStart = (rowsPerProcExtra* rowsRemain) +
                    ((data->mpi_rank - rowsRemain) * rowsPerProcNom);
    }

    int rowsEnd = rowsStart + rowsToCalc;

    std::cout << "Rank " << data->mpi_rank << " Num " << rowsToCalc << " [" <<
                 rowsStart << ", " << rowsEnd << "] Base I " <<
                 calcIndex(data, rowsStart, 0) << std::endl;

    //Render the scene.
    for( int row = rowsStart; row < rowsEnd; ++row )
    {
        for( int col = 0; col < colsMax; ++col )
        {

            //Calculate the index into the array.
            int baseIndex = calcIndex(data, row, col);

            //Call the function to shade the pixel.
            shadePixel(&(pixels[baseIndex]),row,col,data);
        }

    }

    //Stop the comp. timer
    double computationStop = MPI_Wtime();
    double computationTime = computationStop - computationStart;

    int baseIndex = calcIndex(data, rowsStart, 0);
    int numToSave = calcIndex(data, rowsToCalc, 0);
    std::cout << "Slave " << data->mpi_rank << " Sending " << numToSave <<
                 " Pixesl to index " << baseIndex << std::endl;

    MPI_Send( &(pixels[baseIndex]), numToSave, MPI_FLOAT, 0, MPI_MESSAGE_TAG_PIX , MPI_COMM_WORLD);

}

void slaveStaticStripsVertical(ConfigData* data, float* pixels){

}

void slaveStaticBlocks(ConfigData* data, float* pixels){

}

void slaveStaticCyclesHorizontal(ConfigData* data, float* pixels){

}

void slaveStaticCyclesVertical(ConfigData* data, float* pixels){

}

void slaveDynamic(ConfigData* data, float* pixels){

}

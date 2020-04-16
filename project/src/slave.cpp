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
            slaveStaticStripsHorizontal(data);
            break;
        case PART_MODE_STATIC_STRIPS_VERTICAL:
            slaveStaticStripsVertical(data);
            break;
        case PART_MODE_STATIC_BLOCKS:
            slaveStaticBlocks(data);
            break;
        case PART_MODE_STATIC_CYCLES_HORIZONTAL:
            slaveStaticCyclesHorizontal(data);
            break;
        case PART_MODE_STATIC_CYCLES_VERTICAL:
            slaveStaticCyclesVertical(data);
            break;
        case PART_MODE_DYNAMIC:
            slaveDynamic(data);
            break;
        default:
            std::cout << "This mode (" << data->partitioningMode;
            std::cout << ") is not currently implemented." << std::endl;
            break;
    }
}

void slaveStaticStripsHorizontal(ConfigData* data){
    //Start the computation time timer.
    float computationStart, computationStop, computationTime;
    computationStart = MPI_Wtime();

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

    //int rowsEnd = rowsStart + rowsToCalc;

    //Allocate space for the image on the master.
    float* pixels = new float[ calcIndex(data, rowsToCalc + 1, 0)];

    //Render the scene.
    for( int row = 0; row < rowsToCalc; ++row )
    {
        for( int col = 0; col < colsMax; ++col )
        {

            //Calculate the index into the array.
            int baseIndex = calcIndex(data, row, col);

            //Call the function to shade the pixel.
            shadePixel(&(pixels[baseIndex]),row + rowsStart,col,data);
        }

    }

    //Stop the comp. timer
    computationStop = MPI_Wtime();
    computationTime = computationStop - computationStart;

    int pixToSave = calcIndex(data, rowsToCalc, 0);

    int packetSize = pixToSave + 1; // pixel data plus comp time
    // Make memory for the incoming data.
    float* packet = new float[pixToSave + 1];

    // format the packet
    packet[0] = computationTime;
    memcpy(&(packet[1]), pixels, pixToSave * sizeof(float));

    MPI_Send( packet, packetSize, MPI_FLOAT, 0, MPI_MESSAGE_TAG_PIX, MPI_COMM_WORLD);

    //Delete the pixel data.
    delete[] packet;
    delete[] pixels;
}

void slaveStaticStripsVertical(ConfigData* data){
    //Start the computation time timer.
    float computationStart, computationStop, computationTime;
    computationStart = MPI_Wtime();

    // Vars to improve readability
    int colsMax = data->width;
    int rowsMax = data->height;
    // Number of rows to calcualte
    // Divide rows evenenly amoung processors
    int colsPerProcNom = colsMax/data->mpi_procs;
    // Some slaves needs to do addtitional work for the remainder
    int colsPerProcExtra = colsPerProcNom + 1;
    // Int maths leaves a remainder. The remainder will later be distributed to
    // the first n slaves.
    int colsRemain = colsMax%data->mpi_procs;

    // How many rows each slaves calcs
    int colsToCalc;
    // Where in the array this slave starts
    int colsStart;
    if (data->mpi_rank < colsRemain){
        colsToCalc = colsPerProcExtra;
        colsStart = (colsPerProcExtra* data->mpi_rank);
    } else {
        colsToCalc = colsPerProcNom;
        colsStart = (colsPerProcExtra* colsRemain) +
                    ((data->mpi_rank - colsRemain) * colsPerProcNom);
    }

    //int colsEnd = colsStart + colsToCalc;

    //Allocate space for the image on the master.
    float* pixels = new float[ calcIndexI(data, 0, colsToCalc + 1)];

    //Render the scene.
    // Swap row and col itteration.
    for( int row = 0; row < rowsMax; ++row )
    {
        for( int col = 0; col < colsToCalc; ++col )
        {

            //Calculate the index into the array.
            int baseIndexI = calcIndexI(data, row, col);

            //Call the function to shade the pixel.
            shadePixel(&(pixels[baseIndexI]),row,col + colsStart,data);
        }

    }

    //Stop the comp. timer
    computationStop = MPI_Wtime();
    computationTime = computationStop - computationStart;

    int pixToSave = calcIndexI(data, 0, colsToCalc);

    int packetSize = pixToSave + 1; // pixel data plus comp time
    // Make memory for the incoming data.
    float* packet = new float[pixToSave + 1];

    // format the packet
    packet[0] = computationTime;
    memcpy(&(packet[1]), pixels, pixToSave * sizeof(float));

    MPI_Send( packet, packetSize, MPI_FLOAT, 0, MPI_MESSAGE_TAG_PIX, MPI_COMM_WORLD);

    //Delete the pixel data.
    delete[] packet;
    delete[] pixels;
}

void slaveStaticBlocks(ConfigData* data){
    //Start the computation time timer.
    float computationStart, computationStop, computationTime;
    computationStart = MPI_Wtime();

    BlockInfo blockInfo = BlockInfo(data);

    if (blockInfo.sqrtProc == 0){
        //std::cout << "Error: " << data->mpi_procs << " is not a perfect square to break into blocks" << std::endl;
        return;
    }

    int packetSize = blockInfo.GetPacketSize();
    float* pixels = new float[packetSize];

    //Render the scene.
    for( int row = 0; row < blockInfo.rowsToCalc; ++row )
    {
        for( int col = 0; col < blockInfo.colsToCalc; ++col )
        {

            //Calculate the index into the array.
            int baseIndex = blockInfo.GetIndex(row, col);

            //Call the function to shade the pixel.
            shadePixel(&(pixels[baseIndex]),blockInfo.rowStart+row,blockInfo.colStart +  col,data);
        }

    }

    //Stop the comp. timer
    computationStop = MPI_Wtime();
    computationTime = computationStop - computationStart;

    // Add computation time to the end of the packet
    pixels[packetSize-1] = computationTime;

    // send
    MPI_Send( pixels, packetSize, MPI_FLOAT, 0, MPI_MESSAGE_TAG_PIX, MPI_COMM_WORLD);

    delete[] pixels;
}

void slaveStaticCyclesHorizontal(ConfigData* data){
    //return;
    double computationStart, computationStop, computationTime;

    // Vars to improve readability
    int colsMax = data->width;
    int rowsMax = data->height;
    // Number of rows to calcualte
    // Divide rows evenenly amoung processors
    int rowsPerProc = rowsMax/data->mpi_procs + data->cycleSize;
    //int rowsStart = data->mpi_rank * data->cycleSize;


    int numPix = calcIndex(data, rowsPerProc, 0);
    int packetSize = numPix + 1;
    float* pixels = new float[packetSize];

    computationStart = MPI_Wtime();

    //Render the scene.
    int rowMap = 0;
    for( int row = 0; row < rowsMax; ++row )
    {
        if ((row/data->cycleSize) % data->mpi_procs  == data->mpi_rank){

            for( int col = 0; col < colsMax; ++col )
            {

                //Calculate the index into the array.
                int baseIndex = calcIndex(data, rowMap, col);

                //Call the function to shade the pixel.
                shadePixel(&(pixels[baseIndex]),row,col,data);
            }
            rowMap++;
        }
    }

    computationStop = MPI_Wtime();
    computationTime = computationStop - computationStart;

    //return;

    // Put computation time at the end of the pixel array
    pixels[numPix] = computationTime;
    // Ship it to master
    MPI_Send( pixels, packetSize, MPI_FLOAT, 0, MPI_MESSAGE_TAG_PIX, MPI_COMM_WORLD);
    // Free memory
    delete[] pixels;
}

void slaveStaticCyclesVertical(ConfigData* data){
    slaveStaticCyclesHorizontal(data);
}

void slaveDynamic(ConfigData* data){
    //return;
    MPI_Status status;

    DBlockInfo blockInfo = DBlockInfo(data);
    // start the slaves off with their initial
    int blockID = data->mpi_rank - 1;


    // Fetch packet size from the block info. The 0th item has largest size, so
    // use it for allocation
    blockInfo.UpdateData(data, 0);
    int packetSize = blockInfo.GetPacketSize();
    float* packet = new float[packetSize];

    double computationStart, computationStop, computationTime;
    computationTime = 0;




    while (blockID != -1){
        // Update the blockInfo
        blockInfo.UpdateData(data, blockID);

        // Perform calculations
        computationStart = MPI_Wtime();
        //Render the scene.
        for( int row = 0; row < blockInfo.blockRowNum; ++row )
        {
            for( int col = 0; col < blockInfo.blockColNum; ++col )
            {
                //Calculate the index into the array.
                int baseIndex = blockInfo.GetIndex(row,col);

                //Call the function to shade the pixel.
                shadePixel(&(packet[baseIndex]),row + blockInfo.blockRowStart, col + blockInfo.blockColStart,data);
            }

        }
        //Start the computation time timer.
        computationStop = MPI_Wtime();
        computationTime += computationStop - computationStart;

        // pack the packet with needed data.
        packet[DBlockInfo::D_PACKET_META::D_PACKET_META_BLOCK_ID] = blockID;
        packet[DBlockInfo::D_PACKET_META::D_PACKET_META_SLAVE] = data->mpi_rank;
        packet[DBlockInfo::D_PACKET_META::D_PACKET_META_COMP_T] = computationTime;

        // Send data to master
        MPI_Send( packet, packetSize, MPI_FLOAT, 0, MPI_MESSAGE_TAG_PIX, MPI_COMM_WORLD);

        // Recieve command from master
        MPI_Recv( &blockID, 1, MPI_INT, 0, MPI_MESSAGE_TAG_D_CMD , MPI_COMM_WORLD, &status);
    }


}

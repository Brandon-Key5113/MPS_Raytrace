//This file contains the code that the master process will execute.

#include <iostream>
#include <mpi.h>

#include "RayTrace.h"
#include "master.h"
#include "common.h"

void masterMain(ConfigData* data)
{
    //Depending on the partitioning scheme, different things will happen.
    //You should have a different function for each of the required
    //schemes that returns some values that you need to handle.

    //Allocate space for the image on the master.
    float* pixels = new float[(3 * data->width * data->height)];

    //Execution time will be defined as how long it takes
    //for the given function to execute based on partitioning
    //type.
    double renderTime = 0.0, startTime, stopTime, slaveRenderTime = 0.0;
    double commTime;

	//Add the required partitioning methods here in the case statement.
	//You do not need to handle all cases; the default will catch any
	//statements that are not specified. This switch/case statement is the
	//only place that you should be adding code in this function. Make sure
	//that you update the header files with the new functions that will be
	//called.
	//It is suggested that you use the same parameters to your functions as shown
	//in the sequential example below.
    startTime = MPI_Wtime();
    switch (data->partitioningMode)
    {
        case PART_MODE_NONE:
            //Call the function that will handle this.
            slaveRenderTime = masterSequential(data, pixels);
            break;
        case PART_MODE_STATIC_STRIPS_HORIZONTAL:
            slaveRenderTime = masterStaticStripsHorizontal(data, pixels);
            break;
        case PART_MODE_STATIC_STRIPS_VERTICAL:
            slaveRenderTime = masterStaticStripsVertical(data, pixels);
            break;
        case PART_MODE_STATIC_BLOCKS:
            slaveRenderTime = masterStaticBlocks(data, pixels);
            break;
        case PART_MODE_STATIC_CYCLES_HORIZONTAL:
            slaveRenderTime = masterStaticCyclesHorizontal(data, pixels);
            break;
        case PART_MODE_STATIC_CYCLES_VERTICAL:
            slaveRenderTime = masterStaticCyclesVertical(data, pixels);
            break;
        case PART_MODE_DYNAMIC:
            slaveRenderTime = masterDynamic(data, pixels);
            break;
        default:
            std::cout << "This mode (" << data->partitioningMode;
            std::cout << ") is not currently implemented." << std::endl;
            break;
    }
    stopTime = MPI_Wtime();

    renderTime = stopTime - startTime;
    commTime = renderTime - slaveRenderTime;
    std::cout << "Execution Time: " << renderTime << " seconds" << std::endl << std::endl;

    //Print the times and the c-to-c ratio
	//This section of printing, IN THIS ORDER, needs to be included in all of the
	//functions that you write at the end of the function.
    std::cout << "Total Computation Time: " << slaveRenderTime << " seconds" << std::endl;
    std::cout << "Total Communication Time: " << commTime << " seconds" << std::endl;
    double c2cRatio = commTime / slaveRenderTime;
    std::cout << "C-to-C Ratio: " << c2cRatio << std::endl;

    // Print out data in a format for the spreadsheet. *** to be able to regex match it
    printf("***%d\t%f\t\t%f\n", data->mpi_procs, renderTime, c2cRatio);


    //After this gets done, save the image.
    std::cout << "Image will be save to: ";
    std::string file = generateFileName(data);
    std::cout << file << std::endl;
    savePixels(file, pixels, data);

    printf("--------------------------------------------------------------------------------\n");

    //Delete the pixel data.
    delete[] pixels;
}

void masterDisplayCtoC(double commTime, double compTime){
    //Print the times and the c-to-c ratio
	//This section of printing, IN THIS ORDER, needs to be included in all of the
	//functions that you write at the end of the function.
    std::cout << "Total Computation Time: " << compTime << " seconds" << std::endl;
    std::cout << "Total Communication Time: " << commTime << " seconds" << std::endl;
    double c2cRatio = commTime / compTime;
    std::cout << "C-to-C Ratio: " << c2cRatio << std::endl;
}

double masterSequential(ConfigData* data, float* pixels)
{
    //Start the computation time timer.
    double computationStart = MPI_Wtime();

    //Render the scene.
    for( int i = 0; i < data->height; ++i )
    {
        for( int j = 0; j < data->width; ++j )
        {
            int row = i;
            int column = j;

            //Calculate the index into the array.
            int baseIndex = 3 * ( row * data->width + column );

            //Call the function to shade the pixel.
            shadePixel(&(pixels[baseIndex]),row,column,data);
        }
    }

    //Stop the comp. timer
    double computationStop = MPI_Wtime();
    double computationTime = computationStop - computationStart;

    return computationTime;
}

double masterStaticStripsHorizontal(ConfigData* data, float* pixels){
    MPI_Status status;

    //Start the computation time timer.
    double computationStart, computationStop, computationTime, computationTimeTemp;

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

    //std::cout << "Row Per Proc Nominal " << rowsPerProcNom << std::endl;
    //std::cout << "Row Per Proc Extra " << rowsPerProcExtra << std::endl;
    //std::cout << "Remainder " << rowsRemain << std::endl;
    //std::cout << "Rank " << data->mpi_rank << " Num " << rowsToCalc << " [" <<
    //             rowsStart << ", " << rowsEnd << "] Base I " <<
    //             calcIndex(data, rowsStart, 0) << std::endl;

    computationStart = MPI_Wtime();

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
    computationStop = MPI_Wtime();
    computationTime = computationStop - computationStart;

    // Consolidate all pixels and comm time(Communication)
    int slave = 1;
    int baseIndex = calcIndex(data, rowsToCalc, 0);
    int pixToSave = calcIndex(data, rowsPerProcExtra, 0);
    int packetSize = pixToSave + 1; // pixel data plus comp time
    // Make memory for the incoming data.
    float* packet = new float[pixToSave + 1];

    for (slave = 1; slave < rowsRemain; slave++){
        // read new data
        MPI_Recv( packet, packetSize, MPI_FLOAT, slave, MPI_MESSAGE_TAG_PIX , MPI_COMM_WORLD, &status);
        // parse the packet
        // Grab the computation time
        if (packet[0] > computationTime){
            computationTime = packet[0];
        }
        // Copy pixels into pixel buffer
        memcpy(&(pixels[baseIndex]), &packet[1], pixToSave * sizeof(float));
        baseIndex += pixToSave;
    }
    pixToSave = calcIndex(data, rowsPerProcNom, 0);
    packetSize = pixToSave + 1;
    for (; slave < data->mpi_procs; slave++){
        // read new data
        MPI_Recv( packet, packetSize, MPI_FLOAT, slave, MPI_MESSAGE_TAG_PIX , MPI_COMM_WORLD, &status);
        // parse the packet
        // Grab the computation time
        if (packet[0] > computationTime){
            computationTime = packet[0];
        }
        // Copy pixels into pixel buffer
        memcpy(&(pixels[baseIndex]), &packet[1], pixToSave * sizeof(float));
        baseIndex += pixToSave;
    }

    delete[] packet;

    return computationTime;
}

double masterStaticStripsVertical(ConfigData* data, float* pixels){
    MPI_Status status;

    //Start the computation time timer.
    double computationStart, computationStop, computationTime, computationTimeTemp;

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

    int colsEnd = colsStart + colsToCalc;

    //std::cout << "Row Per Proc Nominal " << colsPerProcNom << std::endl;
    //std::cout << "Row Per Proc Extra " << colsPerProcExtra << std::endl;
    //std::cout << "Remainder " << colsRemain << std::endl;
    //std::cout << "Rank " << data->mpi_rank << " Num " << colsToCalc << " [" <<
    //             colsStart << ", " << colsEnd << "] Base I " <<
    //             calcIndex(data, colsStart, 0) << std::endl;

    computationStart = MPI_Wtime();

    float* pixels2 = new float[(3 * data->width * data->height)];

    //Render the scene.
    // Swap row and col itteration.
    for( int row = 0; row < rowsMax; ++row )
    {
        for( int col = colsStart; col < colsEnd; ++col )
        {

            //Calculate the index into the array.
            int baseIndexI = calcIndexI(data, row, col);

            //Call the function to shade the pixel.
            shadePixel(&(pixels2[baseIndexI]),row,col,data);
        }

    }

    //Stop the comp. timer
    computationStop = MPI_Wtime();
    computationTime = computationStop - computationStart;

    // Consolidate all pixels and comm time(Communication)
    int slave = 1;
    int baseIndex = calcIndexI(data, 0, colsToCalc);
    int pixToSave = calcIndexI(data, 0, colsPerProcExtra);
    int packetSize = pixToSave + 1; // pixel data plus comp time
    // Make memory for the incoming data.
    float* packet = new float[pixToSave + 1];

    for (slave = 1; slave < colsRemain; slave++){
        // read new data
        MPI_Recv( packet, packetSize, MPI_FLOAT, slave, MPI_MESSAGE_TAG_PIX , MPI_COMM_WORLD, &status);
        // parse the packet
        // Grab the computation time
        if (packet[0] > computationTime){
            computationTime = packet[0];
        }
        // Copy pixels into pixel buffer
        memcpy(&(pixels2[baseIndex]), &packet[1], pixToSave * sizeof(float));
        baseIndex += pixToSave;
    }
    pixToSave = calcIndex(data, colsPerProcNom, 0);
    packetSize = pixToSave + 1;
    for (; slave < data->mpi_procs; slave++){
        // read new data
        MPI_Recv( packet, packetSize, MPI_FLOAT, slave, MPI_MESSAGE_TAG_PIX , MPI_COMM_WORLD, &status);
        // parse the packet
        // Grab the computation time
        if (packet[0] > computationTime){
            computationTime = packet[0];
        }
        // Copy pixels into pixel buffer
        memcpy(&(pixels2[baseIndex]), &packet[1], pixToSave * sizeof(float));
        baseIndex += pixToSave;
    }

    // Flop the pixels
    for( int i = 0; i < data->height; ++i )
    {
        for( int j = 0; j < data->width; ++j )
        {
            int row = i;
            int column = j;

            //Calculate the index into the array.
            int baseIndex = calcIndex(data, row, column);
            int baseIndexI = calcIndexI(data, row, column);

            pixels[baseIndex] = pixels2[baseIndexI];
            pixels[baseIndex+1] = pixels2[baseIndexI+1];
            pixels[baseIndex+2] = pixels2[baseIndexI+2];
        }
    }

    delete[] packet;

    return computationTime;
}

double masterStaticBlocks(ConfigData* data, float* pixels){
    MPI_Status status;
    //Start the computation time timer.
    double computationStart, computationStop, computationTime, computationTimeTemp;

    computationStart = MPI_Wtime();

    BlockInfo blockInfo = BlockInfo(data);

    if (blockInfo.sqrtProc == 0){
        std::cout << "Error: " << data->mpi_procs << " is not a perfect square to break into blocks" << std::endl;
        return 0.0;
    }

    //for (int slave = 0; slave < data->mpi_procs; slave++){
    //blockInfo.UpdateData(slave);
    //Render the scene.
    for( int row = blockInfo.rowStart; row < blockInfo.rowEnd; ++row )
    {
        for( int col = blockInfo.colStart; col < blockInfo.colEnd; ++col )
        {

            //Calculate the index into the array.
            int baseIndex = calcIndex(data, row, col);

            //Call the function to shade the pixel.
            shadePixel(&(pixels[baseIndex]),row,col,data);
        }

    }
    //}

    //Stop the comp. timer
    computationStop = MPI_Wtime();
    computationTime = computationStop - computationStart;

    // Get the block info for the last slave
    // This will have the largest block size, so use it to alloc memory
    blockInfo.UpdateData(data->mpi_procs - 1);
    int packetSize = blockInfo.GetPacketSize(data);
    float* packet = new float[packetSize];

    int packetIndex;
    int pixelIndex;
    int pixToCopy;

    for (int slave = 1; slave < data->mpi_procs; slave++){
        blockInfo.UpdateData(slave);
        packetSize = blockInfo.GetPacketSize(data);

        // read new data
        MPI_Recv( packet, packetSize, MPI_FLOAT, slave, MPI_MESSAGE_TAG_PIX , MPI_COMM_WORLD, &status);
        // parse the packet
        // Grab the computation time
        if (packet[packetSize-1] > computationTime){
            computationTime = packet[packetSize-1];
        }

        // Copy packet data into pixel array
        //if(slave ==3)
        {


        for (int row = 0; row < blockInfo.rowsToCalc; row++){
            for (int col = 0; col < blockInfo.colsToCalc; col++){
                // Index into pixel array accounts for the offsets
                pixelIndex = calcIndex(data, blockInfo.rowStart + row, blockInfo.colStart + col);
                // Packet is zero indexed
                packetIndex = blockInfo.GetIndex(row, col);

                pixels[pixelIndex] = packet[packetIndex];
                pixels[pixelIndex+1] = packet[packetIndex+1];
                pixels[pixelIndex+2] = packet[packetIndex+2];
            }

        }
        }

    }

    delete[] packet;

    return computationTime;
}

double masterStaticCyclesHorizontal(ConfigData* data, float* pixels){
    MPI_Status status;

    //Start the computation time timer.
    double computationStart, computationStop, computationTime, computationTimeTemp;

    // Vars to improve readability
    int colsMax = data->width;
    int rowsMax = data->height;
    int rowsPerProc = rowsMax/data->mpi_procs + 2;
    int rowsStart = data->mpi_rank * data->cycleSize;

    int numPix = calcIndex(data, rowsPerProc, 0);
    int packetSize = numPix + 1;
    float* pix = new float[packetSize];


    computationStart = MPI_Wtime();

    //Render the scene.
    int rowMap = 0;
    for( int row = 0; row < rowsMax; ++row )
    {
        if ((row/data->cycleSize) % data->mpi_procs  == data->mpi_rank){
            for( int col = 0; col < colsMax; ++col )
            {
                //Calculate the index into the array.
                int baseIndex = calcIndex(data, row/*rowMap*/, col);

                //Call the function to shade the pixel.
                shadePixel(&(pixels[baseIndex]),row,col,data);
            }
            rowMap++;
        }
    }

    //Stop the comp. timer
    computationStop = MPI_Wtime();
    computationTime = computationStop - computationStart;

    // row in the pixel array
    int pixelRow = 0;
    // row in the packed packet
    int mappedRow = 0;

    for (int slave = 1; slave < data->mpi_procs ; slave++)
    //int slave = 1;
    {
        pixelRow = slave*data->cycleSize;
        mappedRow = 0;
        //Recieve data
        MPI_Recv( pix, packetSize, MPI_FLOAT, slave, MPI_MESSAGE_TAG_PIX, MPI_COMM_WORLD, &status);

        // Fetch computation time at the end of the pixel array
        if (pix[numPix] > computationTime){
            computationTime = pix[numPix];
        }

        // Unpack incoming data
        // go until the end of the
        while(pixelRow < rowsMax){
            for(int i = 0; i < data->cycleSize && pixelRow < rowsMax; i++){
                //std::cout << "Row In Pixels " << pixelRow << " Row in Packet " << mappedRow << std::endl;
                // memcpy row from incoming data to the pixel array
                memcpy(&(pixels[calcIndex(data, pixelRow, 0)]), &(pix[calcIndex(data, mappedRow, 0)]), sizeof(float)*3*data->width );
                mappedRow++;
                pixelRow++;
            }
            pixelRow+= (data->mpi_procs-1)*data->cycleSize;

        }
    }

    delete[] pix;

    return computationTime;
}

double masterStaticCyclesVertical(ConfigData* data, float* pixels){

    return 0.0;
}

double masterDynamic(ConfigData* data, float* pixels){
    return 0.0;
}

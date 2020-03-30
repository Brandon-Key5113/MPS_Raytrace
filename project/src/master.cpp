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

	//Add the required partitioning methods here in the case statement.
	//You do not need to handle all cases; the default will catch any
	//statements that are not specified. This switch/case statement is the
	//only place that you should be adding code in this function. Make sure
	//that you update the header files with the new functions that will be
	//called.
	//It is suggested that you use the same parameters to your functions as shown
	//in the sequential example below.
    switch (data->partitioningMode)
    {
        case PART_MODE_NONE:
            //Call the function that will handle this.
            startTime = MPI_Wtime();
            masterSequential(data, pixels);
            stopTime = MPI_Wtime();
            break;
        case PART_MODE_STATIC_STRIPS_HORIZONTAL:
            startTime = MPI_Wtime();
            masterStaticStripsHorizontal(data, pixels);
            stopTime = MPI_Wtime();
            break;
        case PART_MODE_STATIC_STRIPS_VERTICAL:
            startTime = MPI_Wtime();
            masterStaticStripsVertical(data, pixels);
            stopTime = MPI_Wtime();
            break;
        case PART_MODE_STATIC_BLOCKS:
            startTime = MPI_Wtime();
            masterStaticBlocks(data, pixels);
            stopTime = MPI_Wtime();
            break;
        case PART_MODE_STATIC_CYCLES_HORIZONTAL:
            startTime = MPI_Wtime();
            masterStaticCyclesHorizontal(data, pixels);
            stopTime = MPI_Wtime();
            break;
        case PART_MODE_STATIC_CYCLES_VERTICAL:
            startTime = MPI_Wtime();
            masterStaticCyclesVertical(data, pixels);
            stopTime = MPI_Wtime();
            break;
        case PART_MODE_DYNAMIC:
            startTime = MPI_Wtime();
            masterDynamic(data, pixels);
            stopTime = MPI_Wtime();
            break;
        default:
            std::cout << "This mode (" << data->partitioningMode;
            std::cout << ") is not currently implemented." << std::endl;
            break;
    }

    renderTime = stopTime - startTime;
    std::cout << "Execution Time: " << renderTime << " seconds" << std::endl << std::endl;

    //After this gets done, save the image.
    std::cout << "Image will be save to: ";
    std::string file = generateFileName(data);
    std::cout << file << std::endl;
    savePixels(file, pixels, data);

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

void masterSequential(ConfigData* data, float* pixels)
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

    //After receiving from all processes, the communication time will
    //be obtained.
    double communicationTime = 0.0;

    masterDisplayCtoC(communicationTime, computationTime);
}

void masterStaticStripsHorizontal(ConfigData* data, float* pixels){
    MPI_Status status;

    //Start the computation time timer.
    float computationStart, computationStop, computationTime, computationTimeTemp;
    float commStart, commStop, commTime;

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

    std::cout << "Row Per Proc Nominal " << rowsPerProcNom << std::endl;
    std::cout << "Row Per Proc Extra " << rowsPerProcExtra << std::endl;
    std::cout << "Remainder " << rowsRemain << std::endl;
    std::cout << "Rank " << data->mpi_rank << " Num " << rowsToCalc << " [" <<
                 rowsStart << ", " << rowsEnd << "] Base I " <<
                 calcIndex(data, rowsStart, 0) << std::endl;

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
    // Start communicaiton timer
    commStart = MPI_Wtime();

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
        computationTime += packet[0];
        // Copy pixels into pixel buffer
        memcpy(&(pixels[baseIndex]), &packet[1], pixToSave * sizeof(float));
        baseIndex += pixToSave;


        //MPI_Recv( &(pixels[baseIndex]), pixToSave, MPI_FLOAT, slave, MPI_MESSAGE_TAG_PIX , MPI_COMM_WORLD, &status);
        //baseIndex += pixToSave;
        //MPI_Recv( &computationTimeTemp, 1, MPI_FLOAT, slave, MPI_MESSAGE_TAG_COMP_T, MPI_COMM_WORLD, &status);
        //computationTime += computationTimeTemp;
    }
    pixToSave = calcIndex(data, rowsPerProcNom, 0);
    packetSize = pixToSave + 1;
    for (; slave < data->mpi_procs; slave++){
        // read new data
        MPI_Recv( packet, packetSize, MPI_FLOAT, slave, MPI_MESSAGE_TAG_PIX , MPI_COMM_WORLD, &status);
        // parse the packet
        // Grab the computation time
        computationTime += packet[0];
        // Copy pixels into pixel buffer
        memcpy(&(pixels[baseIndex]), &packet[1], pixToSave * sizeof(float));
        baseIndex += pixToSave;
    }

    // Stop communicaiton timer
    commStop = MPI_Wtime();

    //After receiving from all processes, the communication time will
    //be obtained.
    commTime = commStop - commStart;

    masterDisplayCtoC(commTime, computationTime);

    delete[] packet;
}

void masterStaticStripsVertical(ConfigData* data, float* pixels){

}

void masterStaticBlocks(ConfigData* data, float* pixels){

}

void masterStaticCyclesHorizontal(ConfigData* data, float* pixels){

}

void masterStaticCyclesVertical(ConfigData* data, float* pixels){

}

void masterDynamic(ConfigData* data, float* pixels){

}

#ifndef __MASTER_PROCESS_H__
#define __MASTER_PROCESS_H__

#include "RayTrace.h"

//This function is the main that only the master process
//will run.
//
//Inputs:
//    data - the ConfigData that holds the scene information.
//
//Outputs: None
void masterMain( ConfigData *data );

// Helper fn to print c to c text
void masterDisplayCtoC(double commTime, double compTime);

//This function will perform ray tracing when no MPI use was
//given.
//
//Inputs:
//    data - the ConfigData that holds the scene information.
//
//Outputs: Communication time
double masterSequential(ConfigData *data, float* pixels);
double masterStaticStripsHorizontal(ConfigData* data, float* pixels);

double masterStaticStripsVertical(ConfigData* data, float* pixels);

double masterStaticBlocks(ConfigData* data, float* pixels);

double masterStaticCyclesHorizontal(ConfigData* data, float* pixels);

double masterStaticCyclesVertical(ConfigData* data, float* pixels);

double masterDynamic(ConfigData* data, float* pixels);

#endif

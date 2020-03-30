#ifndef __SLAVE_PROCESS_H__
#define __SLAVE_PROCESS_H__

#include "RayTrace.h"

void slaveMain( ConfigData *data );

void slaveStaticStripsHorizontal(ConfigData* data, float* pixels);
void slaveStaticStripsVertical(ConfigData* data, float* pixels);
void slaveStaticBlocks(ConfigData* data, float* pixels);
void slaveStaticCyclesHorizontal(ConfigData* data, float* pixels);
void slaveStaticCyclesVertical(ConfigData* data, float* pixels);
void slaveDynamic(ConfigData* data, float* pixels);

#endif

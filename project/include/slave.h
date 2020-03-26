#ifndef __SLAVE_PROCESS_H__
#define __SLAVE_PROCESS_H__

#include "RayTrace.h"

void slaveMain( ConfigData *data );

void slaveStaticStripsHorizontal(ConfigData* data);
void slaveStaticStripsVertical(ConfigData* data);
void slaveStaticBlocks(ConfigData* data);
void slaveStaticCyclesHorizontal(ConfigData* data);
void slaveStaticCyclesVertical(ConfigData* data);
void slaveDynamic(ConfigData* data);

#endif

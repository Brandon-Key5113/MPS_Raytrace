#include "common.h"

int calcIndex(ConfigData* data,int row,int col){
    3 * ( row * data->width + col );
}

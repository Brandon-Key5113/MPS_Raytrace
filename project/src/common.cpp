#include "common.h"

int calcIndex(ConfigData* data,int row,int col){
    return 3 * ( row * data->width + col );
}

#include "common.h"

int calcIndex(const ConfigData* data,int row,int col){
    return 3 * ( row * data->width + col );
}

int calcIndexI(const ConfigData* data,int row,int col){
    return 3 * ( col * data->height + row );
}


int isPerfectSquare(int n)
{
    int square = 1;
    int i = 1;
    while (square <= n){
        square = i*i;
        if (square == n){
            return i;
        }
        ++i;
    }
    return 0;
}

BlockInfo::BlockInfo(const ConfigData* data){
    sqrtProc = isPerfectSquare(data->mpi_procs);
    if (sqrtProc == 0){
        return;
    }

    colsMax = data->width;
    rowsMax = data->height;


    slave = data->mpi_rank;

    rowsNorm = rowsMax/sqrtProc;
    rowsExtra = rowsNorm+1;
    rowsRemain = rowsMax%sqrtProc;
    rowsRemainStart = rowsMax - rowsRemain;
    colsNorm = colsMax/sqrtProc;
    colsExtra = colsNorm+1;
    colsRemain = colsMax%sqrtProc;
    colsRemainStart = colsMax - colsRemain;

    UpdateData(slave);
}

void BlockInfo::UpdateData(int slave){
    this->slave = slave;

    blockRow = slave/sqrtProc;
    blockCol = slave%sqrtProc;

    if (blockRow >= rowsRemainStart){
        rowsToCalc = rowsExtra;
        // Accumulate the normal sized blocks and then the additional
        // blocks that are larger
        rowStart = rowsRemainStart*rowsNorm + (blockRow - rowsRemainStart)*rowsExtra;
    } else {
        rowsToCalc = rowsNorm;
        rowStart = blockRow*rowsNorm;
    }
    rowEnd = rowStart+rowsToCalc;

    if (blockRow >= colsRemainStart){
        colsToCalc = colsExtra;
        // Accumulate the normal sized blocks and then the additional
        // blocks that are larger
        colStart = colsRemainStart*colsNorm + (blockCol - colsRemainStart)*colsExtra;
    } else {
        colsToCalc = colsNorm;
        colStart = blockCol*colsNorm;
    }
    colEnd = colStart+colsToCalc;
}

int BlockInfo::GetNumPix(const ConfigData* data){
    return 3*(rowsToCalc+1)*colsToCalc;
}

int BlockInfo::GetPacketSize(const ConfigData* data){
    return GetNumPix(data) + 1;
}

int BlockInfo::GetIndex(int row, int col){
    return 3 * ( row * colsToCalc + col );
}

#include "common.h"

// For maths
#include <algorithm>

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


    blockID = data->mpi_rank;

    rowsNorm = rowsMax/sqrtProc;
    rowsExtra = rowsNorm+1;
    rowsRemain = rowsMax%sqrtProc;
    rowsRemainStart = rowsMax - rowsRemain;
    colsNorm = colsMax/sqrtProc;
    colsExtra = colsNorm+1;
    colsRemain = colsMax%sqrtProc;
    colsRemainStart = colsMax - colsRemain;

    UpdateData(blockID);
}

void BlockInfo::UpdateData(int blockID){
    this->blockID = blockID;

    blockRow = blockID/sqrtProc;
    blockCol = blockID%sqrtProc;

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


DBlockInfo::DBlockInfo(const ConfigData* data){
    // Populate general block properties

    // User defined block size
    blockHeight = data->dynamicBlockHeight;
    blockWidth = data->dynamicBlockWidth;

    // Image size in blocks
    numBlocksTall = ceilDiv(  data->height, blockHeight);
    numBlocksWide = ceilDiv(  data->width, blockWidth);

    // Populate block specific data from the rank
    UpdateData(data, data->mpi_rank);
}


void DBlockInfo::UpdateData(const ConfigData* data, int blockID){
    this->blockID = blockID;

    // Coordinates of this block
    blockIDx = blockID/numBlocksWide;
    blockIDy = blockID%numBlocksWide;

    // Calc the block coordinates in pixels
    // upper left corner
    blockColStart = blockIDx*blockWidth;
    blockRowStart = blockIDy*blockHeight;

    // lower right corner
    blockColEnd = std::min(blockColStart+blockWidth, data->width );
    blockRowEnd = std::min(blockRowStart+blockHeight, data->height );

    // dimensions
    blockColNum = blockColEnd - blockColStart;
    blockRowNum = blockRowEnd - blockRowStart;
}

int DBlockInfo::GetNumPix(){
    return 3*blockColNum*blockRowNum;
}

int DBlockInfo::GetPacketSize(){
    return GetNumPix() + 2;
}

int DBlockInfo::GetIndex(int row, int col){
    return 3 * ( row * blockColNum + col );
}



#ifndef __COMMON_H__
#define __COMMON_H__

#include "RayTrace.h"

typedef enum{
    MPI_MESSAGE_TAG_PIX = 8,
    MPI_MESSAGE_TAG_D_CMD
} MpiMesssageTag;


typedef struct BlockInfo{
    // Square root of the number of procs
    // How many blocks wide the image is
    int sqrtProc;

    // Image size
    int colsMax;
    int rowsMax;
    // Index of the block
    int blockID;

    int rowsNorm;
    int rowsExtra;
    int rowsRemain;
    int rowsRemainStart;
    int colsNorm;
    int colsExtra;
    int colsRemain;
    int colsRemainStart;


    // Coordinates of the block
    int blockRow;
    int blockCol;

    // For pixel mapping
    int rowStart, rowEnd, rowsToCalc;
    int colStart, colEnd, colsToCalc;

    // Construct a new BlockInfo
    BlockInfo(const ConfigData* data);

    // Update the info if changing block. Useful for when the master is recieving
    // and needs the info updated, but doesn't need a whole new blockinfo
    void UpdateData(int blockID);
    // Number of pixels represented by this block. Already multiplied by 3 for
    // the size in floats
    int GetNumPix();
    // Like num pix, but with padding for extra data to be sent
    int GetPacketSize();
    // Get the index into a pixel array that represents this block
    int GetIndex(int row, int col);


} BlockInfo;

// Dynamic block info
typedef struct DBlockInfo{

    // User defined block size
    // Not the acutal size of each block.
    // Blocks on the end will be partial sized.
    int blockHeight, blockWidth;


    // Mapping to pixels
    int blockRowStart, blockColStart;
    int blockRowNum, blockColNum;
    int blockRowEnd, blockColEnd;

    // Number of blocks per image
    int numBlocksWide, numBlocksTall;

    int blockID;
    int blockIDx, blockIDy;

    // Constructs a dynamic block info
    DBlockInfo(const ConfigData* data);

    // Update the info if changing block. Useful for when the master is recieving
    // and needs the info updated, but doesn't need a whole new blockinfo
    void UpdateData(const ConfigData* data, int blockID);
    // Number of pixels represented by this block. Already multiplied by 3 for
    // the size in floats
    int GetNumPix();
    // Like num pix, but with padding for extra data to be sent
    int GetPacketSize();
    // Get the index into a pixel array that represents this block
    int GetIndex(int row, int col);


    typedef enum{
        D_PACKET_META_SLAVE = 0,
        D_PACKET_META_BLOCK_ID,
        D_PACKET_META_COMP_T,
        D_PACKET_META_MAX
    } D_PACKET_META;

} DBlockInfo;

//This function is a helper function to calculate an index into a pixel array
//
//Inputs:
//    data - the ConfigData that holds the scene information.
//    row - the pixel row
//    col - the pixel column
//
//Outputs: index
int calcIndex(const ConfigData* data,int row,int col);
//Column major index calc
int calcIndexI(const ConfigData* data,int row,int col);

//Math function to check if a number is a perfect square
//
//Inputs:
//    n - Number to check
//
//Outputs:
//    0 - Number is not a perfect square
//    other - square root of the number
int isPerfectSquare(int n);

// Returns ceil(x/y)
inline int ceilDiv(int x, int y){
    return (x + y - 1) / y;
}

#endif

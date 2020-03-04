#ifndef MD5_H
#define MD5_H

#include <inttypes.h>

const int BLOCK_SIZE = 512 / 8;
const int RESERVED_BYTES = 8;

typedef struct HashInfo {

    // User set members
    long inputBytes;

    // Calculated members
    long totalBytesNeeded;

    // Blocks completely filled with input bytes
    long fullBlocks;
    int paddedBlocks;

    int bytesNotInFullBlocks;


    long bytesInFullBlocks;
    int bytesInLastBlock;

    int bytesInPaddedBlocks;

    int hasFinalEmptyBlock;

} HashInfo;

#endif
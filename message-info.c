#include "message-info.h"

const int BLOCK_SIZE = 512 / 8;
const int RESERVED_BYTES = 8;

void printHashInfo(HashInfo *h) {

    printf("----------------------------------\n");
    printf("    Block Size: %d Bytes\n", BLOCK_SIZE);
    printf("  Input Length: %d Bytes\n", h->inputBytes);
    printf("      Reserved: %d Bytes\n", RESERVED_BYTES);
    printf("  Total Needed: %d Bytes\n", h->totalBytesNeeded);
    printf("----------------------------------\n");
    printf("   Full Blocks: %ld\n", h->fullBlocks);
    printf("----------------------------------\n");
}

void calculateHashInfo(HashInfo *h, long inputBytes) {
    printf("\nCalculating message info...\n");
    HashInfo hashInfo;
    h->inputBytes = inputBytes;
    h->totalBytesNeeded = inputBytes + RESERVED_BYTES;
    h->fullBlocks = floor((double)(inputBytes) / BLOCK_SIZE);
}

long getFileBytes(FILE *file) {
    // Get the size of the file in bytes
    fseek(file, 0L, SEEK_END);
    long numInputBytes = ftell(file);
    fseek(file, 0L, 0L);

    return numInputBytes;
}

void getFileInfo(FILE *file, HashInfo *h)  {
    calculateHashInfo(h, getFileBytes(file));
    printHashInfo(h);
}
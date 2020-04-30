#include "message-info.h"

const int BLOCK_SIZE = 512 / 8;
const long RESERVED_BYTES = 8;

void printHashInfo(HashOptions hashOptions)
{

    HashInfo h;
    long numBytes = 0;
    if (hashOptions.isFile == true)
    {
        numBytes = getFileBytes(hashOptions.file);
    }
    else
    {
        numBytes = strlen(hashOptions.string);
    }

    printf("\nCalculating message info...\n");
    calculateHashInfo(&h, numBytes);

    printf("----------------------------------\n");
    printf("    Block Size: %d Bytes\n", BLOCK_SIZE);
    printf("  Input Length: %d Bytes\n", h.inputBytes);
    printf("      Reserved: %d Bytes\n", RESERVED_BYTES);
    printf("  Total Needed: %d Bytes\n", h.totalBytesNeeded);
    printf("   Full Blocks: %ld\n", h.fullBlocks);
    printf("----------------------------------\n");
}

long getFileBytes(FILE *file)
{
    // Get the size of the file in bytes
    fseek(file, 0L, SEEK_END);
    long numInputBytes = ftell(file);
    fseek(file, 0L, 0L);

    return numInputBytes;
}

void calculateHashInfo(HashInfo *h, long inputBytes)
{
    h->inputBytes = inputBytes;
    long totalBytes = inputBytes + RESERVED_BYTES;
    h->totalBytesNeeded = inputBytes + RESERVED_BYTES;
    h->fullBlocks = floor((double)(inputBytes) / BLOCK_SIZE);
}
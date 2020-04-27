#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <stdio.h>
#include <math.h>
#include <inttypes.h>

typedef struct HashInfo {
    // User set members
    long inputBytes;
    // Calculated members
    long totalBytesNeeded;
    // Blocks completely filled with input bytes
    long fullBlocks;
} HashInfo;

void printHashInfo(HashInfo *h);

void calculateHashInfo(HashInfo *h, long inputBytes);

long getFileBytes(FILE *file);

void getFileInfo(FILE *file, HashInfo *h);

#endif
#ifndef FILE_INFO_H
#define FILE_INFO_H

#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>

#include "custom-types.h"

typedef struct HashInfo
{
    // User set members
    long inputBytes;
    // Calculated members
    long totalBytesNeeded;
    // Blocks completely filled with input bytes
    long fullBlocks;
} HashInfo;

void printHashInfo(HashOptions hashOptions);

static void calculateHashInfo(HashInfo *h, long inputBytes);

static long getFileBytes(FILE *file);

static void getFileInfo(FILE *file, HashInfo h);

#endif
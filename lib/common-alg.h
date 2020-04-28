#ifndef COMMON_ALG_H
#define COMMON_ALG_H

#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

#include "custom-types.h"

// Function pointers in C
// https://stackoverflow.com/questions/840501/how-do-function-pointers-in-c-work/840504#840504

int processStringBlock(MessageBlock *M, char *buffer, size_t *numBits, enum HashStatus *status,
                       int (*processNextBlock)(MessageBlock *M, size_t numBytesRead, size_t *numBits, enum HashStatus *status));

int processFileBlock(MessageBlock *M, FILE *infile, size_t *numBits, enum HashStatus *status,
                     int (*processNextBlock)(MessageBlock *M, size_t numBytesRead, size_t *numBits, enum HashStatus *status));

size_t getBytesFromString(char buffer[], char *inStr, int start, int length);

#endif
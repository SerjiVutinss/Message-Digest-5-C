#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>

#include "lib/md5.h"
#include "lib/sha256.h"
#include "lib/custom-types.h"
#include "lib/message-info.h"

WORD *hash(HashOptions hashOptions);
void initialiseSupportedAlgorithms();
void printHelp();
void getHashChars(char *str, WORD *hash, AlgorithmEnum algorithm);

int getOptions(HashOptions *hashOptions, int argc, char *argv[]);

AlgorithmType getAlgorithmType(AlgorithmEnum algorithm);
int quit();

#endif
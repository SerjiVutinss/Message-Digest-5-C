#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H

#include <stdio.h>
#include <stdbool.h>
#include <inttypes.h>

/**
 * Enum type used to track the status of a hashing algorithm.
 **/
typedef enum HashStatus
{
    READ,
    PAD_ZEROES,
    FINISHED
} HashStatus;

/**
 * Define a WORD type which is an alias for a 32 bit unsigned integer type.
 **/
#define WORD uint32_t

/**
 * Define a union type, Block, which represents a 512 bit message block to be
 * processed by the algorithm.
 **/
typedef union MessageBlock {
    uint64_t sixty_four[8];
    uint32_t thirty_two[16];
    uint8_t eight[64];
} MessageBlock;

typedef enum AlgorithmEnum
{
    MD5,
    SHA256,
    BOTH
} AlgorithmEnum;

typedef struct AlgorithmType
{
    AlgorithmEnum algorithm;
    char name[16];
    char flag[10];
} AlgorithmType;

typedef struct HashOptions
{
    bool isFile;
    FILE *file;
    bool isString;
    char *string;
    bool isVerbose;
    bool isOutputToFile;
    char *outputFilename;
    AlgorithmEnum algorithm;

} HashOptions;

#endif
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

AlgorithmType algorithm;

int quit()
{
    printf("Aborting!\n");
    return 1;
}

struct AlgorithmType supportedAlgorithms[3];

void initialise()
{
    struct AlgorithmType md5;
    struct AlgorithmType sha256;
    struct AlgorithmType both;

    md5.algorithm = MD5;
    strcpy(md5.name, "MD5");
    strcpy(md5.flag, "--md5");

    sha256.algorithm = SHA256;
    strcpy(sha256.name, "SHA256");
    strcpy(sha256.flag, "--sha256");

    both.algorithm = BOTH;
    strcpy(both.name, "MD5 and SHA256");
    strcpy(both.flag, "--both");

    supportedAlgorithms[0] = md5;
    supportedAlgorithms[1] = sha256;
    supportedAlgorithms[2] = both;
}

void printHelp()
{
    printf("\n\nHELP\n\n");
}

bool isVerbose;
bool isOutput;
char *outputFileName;

int main(int argc, char *argv[])
{
    initialise();

    // Expect at least one supplied argument, print the help screen and
    // error out of program if not received.
    if (argc <= 1 || strcmp("--help", argv[1]) == 0)
    {
        printf("Error: not enough arguments supplied.\n");
        printf("Error: expected at least filename argument.\n");

        printHelp();
        return 1;
    }

    // Get the filename argument.
    char *filename = argv[1];
    printf("Opening file: %s\n", filename);

    // Try to open the file
    FILE *inFile = fopen(argv[1], "rb");
    if (!inFile)
    {
        printf("Error: couldn't open file %s\n", argv[1]);
        return quit();
    }
    else
    {
        printf("Successfully opened file: %s\n", filename);
    }

    // parse the remaining arguments
    for (int i = 2; i < argc; i++)
    {
        // Check for verbose argument
        if (strcmp("--verbose", argv[i]) == 0)
        {
            isVerbose = true;
        }

        for (int j = 0; j < 3; j++)
        {
            // printf("Checking for %s flag\n", supportedAlgorithms[j].name);
            if (strcmp(supportedAlgorithms[j].flag, argv[i]) == 0)
            {
                // printf("FOUND");
                algorithm = supportedAlgorithms[j];
            }
        }

        if (strcmp("--output", argv[i]) == 0)
        {
            isOutput = true;
            outputFileName = argv[i + 1];
        }
    }

    printf("Using algorithm: %s\n", algorithm.name);
    char *strVerbose = (isVerbose) ? "Enabled" : "Disabled";
    char *strOutput = (isOutput) ? "Enabled" : "Disabled";
    printf("Verbose mode: %s\n", strVerbose);
    printf("Output mode: %s\n", strOutput);
    if (isOutput)
    {
        printf("Output file: %s\n", outputFileName);
    }

    if (isVerbose)
    {
        HashInfo hashInfo;
        getFileInfo(inFile, &hashInfo);
    }

    WORD *result = startMD5Hash(inFile);

    if (isOutput)
    {
        // printf("OUTPUTTING TO FILE %s\n", outputFileName);
        FILE *outFile = fopen(outputFileName, "w+");
        // printf("OPENED FILE %s\n", outputFileName);
        for (int i = 0; i < 4; i++)
        {
            // printf("WRITING TO FILE %s\n", outputFileName);
            fprintf(outFile, "%02x%02x%02x%02x", (result[i] >> 0) & 0xFF, (result[i] >> 8) & 0xFF, (result[i] >> 16) & 0xFF, (result[i] >> 24) & 0xFF);
            // fprintf(outFile, "Hello....");
        }
        fclose(outFile);
    }

    // if (algorithm == 0)
    // {
    //     startMD5Hash(inFile);
    // }
    // else
    // {
    //     startSHA256(inFile);
    // }

    return 0;
}
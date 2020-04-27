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

    // both.algorithm = BOTH;
    // strcpy(both.name, "MD5 and SHA256");
    // strcpy(both.flag, "--both");

    supportedAlgorithms[0] = md5;
    supportedAlgorithms[1] = sha256;
    // supportedAlgorithms[2] = both;
}

AlgorithmType getAlgorithmType(AlgorithmEnum algorithm)
{
    for (int i = 0; i < 2; i++)
    {
        if (supportedAlgorithms[i].algorithm == algorithm)
        {
            return supportedAlgorithms[i];
        }
    }
}

void printHelp()
{
    printf("\n\nHELP\n\n");
}

int main(int argc, char *argv[])
{
    initialise();

    // for (int i = 0; i < argc; i++)
    // {
    //     printf("ARG: %s\n", argv[i]);
    // }

    // Expect at least one supplied argument, print the help screen and
    // error out of program if not received.
    if (argc <= 1 || strcmp("--help", argv[1]) == 0)
    {
        printf("Error: not enough arguments supplied.\n");
        printf("Error: expected at least inputArg argument.\n");

        printHelp();
        return 1;
    }

    // Create a new options struct to hold the arguments
    HashOptions hashOptions;
    hashOptions.isFile = false;
    hashOptions.isString = false;
    hashOptions.isOutputToFile = false;
    hashOptions.isVerbose = false;

    int argIndexParsed;
    // Get the first user arg
    char *inputArg = argv[1];

    // Check if the string option was passed
    if (strcmp("--string", inputArg) == 0)
    {
        argIndexParsed = 1;
        if (argc <= 2)
        {
            // Cannot proceed because another arg was expected directly after the --string option
            printf("Did not find required arg following option --string!\n");
            return quit();
        }
        // Set the options accordingly...
        hashOptions.isString = true;
        // Since --string must be the first arg, the second arg must be the string to hash
        hashOptions.string = strcat(argv[argIndexParsed + 1], "\0");
        argIndexParsed++;
    }
    else
    {
        // This must just be a filename...
        hashOptions.isFile = true;
        // Try to open the file
        char *filename = argv[argIndexParsed + 1];
        printf("Opening file: %s\n", inputArg);
        hashOptions.file = fopen(argv[argIndexParsed + 1], "rb");
        if (!hashOptions.file)
        {
            printf("Error: couldn't open file %s\n", filename);
            return quit();
        }
        else
        {
            printf("Successfully opened file: %s\n", filename);
        }
        argIndexParsed++;
    }

    // parse the remaining arguments
    for (int i = argIndexParsed; i < argc; i++)
    {
        // Check for verbose argument
        if (strcmp("--verbose", argv[i]) == 0)
        {
            hashOptions.isVerbose = true;
        }

        for (int j = 0; j < 2; j++)
        {
            // printf("Checking for %s flag\n", supportedAlgorithms[j].name);
            if (strcmp(supportedAlgorithms[j].flag, argv[i]) == 0)
            {
                // printf("FOUND");
                hashOptions.algorithm = supportedAlgorithms[j].algorithm;
            }
        }

        if (strcmp("--output", argv[i]) == 0)
        {
            hashOptions.isOutputToFile = true;
            hashOptions.outputFilename = argv[i + 1];
        }
    }

    printf("Using algorithm: %s\n", getAlgorithmType(hashOptions.algorithm).name);
    char *strVerbose = (hashOptions.isVerbose) ? "Enabled" : "Disabled";
    char *strOutput = (hashOptions.isOutputToFile) ? "Enabled" : "Disabled";
    printf("Verbose mode: %s\n", strVerbose);
    printf("Output mode: %s\n", strOutput);

    if (hashOptions.isOutputToFile)
    {
        printf("Output file: %s\n", hashOptions.outputFilename);
    }

    if (hashOptions.isVerbose && hashOptions.isFile)
    {
        printf("GETTING FILE INFO\n");
        HashInfo hashInfo;
        getFileInfo(hashOptions.file, &hashInfo);
    }

    if (hashOptions.algorithm == SHA256)
    {
        WORD *result = startSHA256Hash(hashOptions);

        for (int i = 0; i < 8; i++)
            printf("%08" PRIx32 "", result[i]);
        printf("\n");

        printf("\n");

        if (hashOptions.isOutputToFile)
        {
            // printf("OUTPUTTING TO FILE %s\n", outputFileName);
            FILE *outFile = fopen(hashOptions.outputFilename, "w+");
            // printf("OPENED FILE %s\n", outputFileName);
            for (int i = 0; i < 4; i++)
            {
                // printf("WRITING TO FILE %s\n", outputFileName);
                fprintf(outFile, "%02x%02x%02x%02x", (result[i] >> 0) & 0xFF, (result[i] >> 8) & 0xFF, (result[i] >> 16) & 0xFF, (result[i] >> 24) & 0xFF);
                // fprintf(outFile, "Hello....");
            }
            fclose(outFile);
        }
    }
    else // MD5
    {
        WORD *result = startMD5Hash(hashOptions);
        printf("\n");
        for (int i = 0; i < 4; i++)
        {
            printf("%02x%02x%02x%02x", (result[i] >> 0) & 0xFF, (result[i] >> 8) & 0xFF, (result[i] >> 16) & 0xFF, (result[i] >> 24) & 0xFF);
        }
        printf("\n");
    }

    return 0;
}
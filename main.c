#include "main.h"

struct AlgorithmType supportedAlgorithms[3];

int main(int argc, char *argv[])
{
    // Set up some variables
    initialiseSupportedAlgorithms();

    // If help was passed in, print the help screen.
    if (argc == 2 && strcmp("--help", argv[1]) == 0)
    {
        printHelp();
        return 1;
    }
    else if (argc <= 1)
    {
        // No arg was passed, print error and then the help screen.
        printf("Error: Not enough arguments supplied");
        printf(" - expected at least a filename or --help.\n");
        printf("Showing --help:\n\n");
        printHelp();

        return 1;
    }

    // Create a struct to hold info parsed from the args.
    // The file is also opened and assigned to hashOptions.file.
    HashOptions hashOptions;
    if (getOptions(&hashOptions, argc, argv) == 1)
    {
        // If getOptions returns 1, an error was encountered.
        return 1;
    }

    // Print detailed info about the input message, e.g. Blocks required.
    if (hashOptions.isFile && hashOptions.isVerbose)
    {
        printHashInfo(hashOptions);
    }

    // Hash the message.
    WORD *result = hash(hashOptions);

    // Get a string representation of the hash.
    char *hashChars;
    getHashChars(hashChars, result, hashOptions.algorithm);

    // And print the hash to file if the option is enabled.
    if (hashOptions.isOutputToFile)
    {
        FILE *outFile = fopen(hashOptions.outputFilename, "w+");
        fprintf(outFile, hashChars);
        fclose(outFile);
        printf("\nSaved hash to: %s\n", hashOptions.outputFilename);
    }

    // Finally, print the hash to the screen.
    printf("\n%s: %s\n", getAlgorithmType(hashOptions.algorithm).name, hashChars);

    return 0;
}

// Decides which Hash algorithm to run based on the hashOptions parameter.
WORD *hash(HashOptions hashOptions)
{
    WORD *result;
    if (hashOptions.algorithm == SHA256)
    {
        result = startSHA256Hash(hashOptions);
    }
    else // MD5
    {
        // Default to MD5
        result = startMD5Hash(hashOptions);
    }
    return result;
}

// Create a string from the contents of hash. This will vary per algorithm type.
void getHashChars(char *str, WORD *hash, AlgorithmEnum algorithm)
{
    int pos = 0;
    if (algorithm == MD5)
    {
        for (int i = 0; i < 4; i++)
        {
            pos += sprintf(&str[pos], "%02x%02x%02x%02x", (hash[i] >> 0) & 0xFF, (hash[i] >> 8) & 0xFF, (hash[i] >> 16) & 0xFF, (hash[i] >> 24) & 0xFF);
        }
    }
    else if (algorithm == SHA256)
    {
        for (int i = 0; i < 8; i++)
        {
            pos += sprintf(&str[pos], "%08" PRIx32 "", hash[i]);
        }
    }
}

// Set up values for each supported algorithm.
void initialiseSupportedAlgorithms()
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

void printHelp()
{
    char programName[] = "hash";
    printf("Usage:\n");
    printf("\tFile:   %s [FILE] [OPTION]...\n", programName);
    printf("\tString: %s --string [STRING] [OPTION]...\n\n", programName);
    printf("Calculate and print out the MD5(default) or SHA256 hash of the input FILE or STRING.\n");
    printf("NOTE: If string hashing is to be used, --string MUST be the first argument\n\n");

    printf("\t--string [STRING] \tUse the next argument as input. --string MUST be the first argument passed if it used.\n");
    printf("\t\t\t\tIf the string includes whitespace, it should be surrounded by quotes.\n");
    printf("\n");

    printf("\t--sha256 \t\tUse the SHA256 algorithm to calculate the hash.\n");
    printf("\n");
    printf("\t--output [OUTPUT]\tWrite the hash to the file, [OUTPUT]. MUST be followed by the output filename.\n");
    printf("\n");
    printf("\t--verbose \t\tPrint detailed hash information to the screen.\n");
    printf("\n");

    printf("Example usage:\n");
    printf("\t$ %s input.txt - hash the contents of input.txt using MD5 and print the hash to the screen. \n", programName);
    printf("\t$ %s --string \"hello world\" - hash the string \"hello world\" and print the hash to the screen. \n", programName);

    printf("\t$ %s input.txt --sha256 - hash the contents of input.txt using SHA256 and print the hash to the screen. \n", programName);
    printf("\t$ %s input.txt --output output.txt - hash the contents of input.txt, print the hash to the screen and also write it to the file output.txt. \n", programName);
    printf("\n");
}

// Parse arguments - if file has been used, the file will be opened in this function.
int getOptions(HashOptions *hashOptions, int argc, char *argv[])
{
    // Create a new options struct to hold the arguments

    hashOptions->isFile = false;
    hashOptions->isString = false;
    hashOptions->isOutputToFile = false;
    hashOptions->isVerbose = false;

    char *filename;

    int argIndexParsed = 0;
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
        hashOptions->isString = true;
        // Since --string must be the first arg, the second arg must be the string to hash
        hashOptions->string = strcat(argv[argIndexParsed + 1], "\0");
        argIndexParsed++;
    }
    else
    {
        // This must just be a filename...
        hashOptions->isFile = true;
        // Try to open the file
        filename = argv[argIndexParsed + 1];

        hashOptions->file = fopen(argv[argIndexParsed + 1], "rb");
        if (!hashOptions->file)
        {
            printf("Error: couldn't open file %s\n", filename);
            return quit();
        }
        else
        {
        }
        argIndexParsed++;
    }

    bool foundAlgorithmArg = false;
    // parse the remaining arguments
    for (int i = argIndexParsed; i < argc; i++)
    {
        // Check for verbose argument
        if (strcmp("--verbose", argv[i]) == 0)
        {
            hashOptions->isVerbose = true;
        }

        if (foundAlgorithmArg == false)
        {
            for (int j = 0; j < 2; j++)
            {
                // printf("Checking for %s flag\n", supportedAlgorithms[j].name);
                if (strcmp(supportedAlgorithms[j].flag, argv[i]) == 0)
                {
                    // printf("FOUND");
                    hashOptions->algorithm = supportedAlgorithms[j].algorithm;
                    foundAlgorithmArg = true;
                }
            }
        }

        if (strcmp("--output", argv[i]) == 0)
        {
            hashOptions->isOutputToFile = true;
            hashOptions->outputFilename = argv[i + 1];
        }
    }

    if (foundAlgorithmArg == false)
    {
        hashOptions->algorithm = MD5;
    }

    // Print this here because if file failed to open, quit() would have been called.
    if (hashOptions->isVerbose == true)
    {
        printf("Successfully opened file: %s\n", filename);
    }

    printf("Using algorithm: %s\n", getAlgorithmType(hashOptions->algorithm).name);
    char *strVerbose = (hashOptions->isVerbose) ? "Enabled" : "Disabled";
    char *strOutput = (hashOptions->isOutputToFile) ? "Enabled" : "Disabled";
    printf("Verbose mode: %s\n", strVerbose);
    printf("Output mode: %s\n", strOutput);

    if (hashOptions->isOutputToFile)
    {
        printf("Output file: %s\n", hashOptions->outputFilename);
    }

    return 0;
}

// Get an algorithm config from an enum type.
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

// Program quit unexpectedely, i.e. due to error.
int quit()
{
    printf("\nAborting!\n\n");
    printHelp();
    return 1;
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "lib/test-lib.h"

// Usage

// ./program

// main 

int handleAlgorithmArg(char *argv[], int index);
int handleOutputArg(char *argv[], int index);

int main(int argc, char *argv[]) {

    printf("Starting...\n");

    // arg 1 is always program?
    if(argc == 1) {

        // must provide a file name

    }

    for(int i = 0; i < argc; i++) {

        int algorithmArg = strcmp(argv[i], "-a");
        int outputArg = strcmp(argv[i], "-o");

        if(algorithmArg == 0) {
            handleAlgorithmArg(argv, i);
            i++;
        }

        if(outputArg == 0) {
            handleOutputArg(argv, i);
            i++;
        }
    }
}

int handleAlgorithmArg(char *argv[], int index) {
    // printf("Got Algorithm Arg at index: %d\n", index);
    // printf("Checking for argument at index: %d\n", index + 1);

    char *arg = argv[index+1];
    if(strlen(arg) > 0) {
        printf("Algorithm %s\n", arg);
        return 1;
    }
    return 0;
}

int handleOutputArg(char *argv[], int index) {
    // printf("Got File Output arge at index: %d\n", index);
    // printf("Checking for argument at index: %d\n", index + 1);

    char *arg = argv[index+1];
    if(strlen(arg) > 0) {
        printf("Output file: %s\n", arg);
        return 1;
    }
    return 0;
}
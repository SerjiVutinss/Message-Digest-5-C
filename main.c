#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

#include "lib/md5.h"
#include "lib/sha256.h"
#include "lib/message-info.h"

void parseArgs() {

}

int main(int argc, char *argv[]) {

    // 0 for SHA256 or 1 for MD5
    int algorithm = 1;

    if(algorithm == 1) printf("--- SHA256 Algorithm---\n");
    else printf("--- MD5 Algorithm---\n");


    // printf("Found %d CLI arguments\n", argc);

    // for(int i = 0; i < argc; i++) {



    //     printf("Arg %d: %s\n", i, argv[i]);
    // }

    // Expect and open a single filename.
    if (argc != 2) {
        printf("Error: expected single filename as argument.\n");
        return 1;
    }

    FILE *inFile = fopen(argv[1], "rb");
    if (!inFile) {
        printf("Error: couldn't open file %s.\n", argv[1]);
        return 1;
    }

    HashInfo hashInfo;
    getFileInfo(inFile, &hashInfo);

    if(algorithm == 0) {
        startMD5Hash(inFile);
    } else {
        startSHA256(inFile);
    }

    return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>

#include "main.h"
#include "sha256.h"

#include "message-info.h"
#include "message-block.h"

void closeBlock(Block *M, WORD *H) { 
    convertBlockToHostEndianness(M);
    nextHashSHA256(M->thirty_two, H);
}

int main(int argc, char *argv[]) {

    printf("--- MD5 Algorithm---\n");


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

    printf("\n");
    printf("Processing: %ld full blocks...\n", hashInfo.fullBlocks);

    // Section 5.3.3
    WORD H[] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19  
    };

    Block M;

    uint64_t len = (uint64_t)hashInfo.inputBytes * 8ULL;

    // Process the full blocks
    for(int i = 0; i < hashInfo.fullBlocks; i++){
        // Read all 64 bytes for this block
        size_t read = fread(M.eight, 1, 64, inFile);
        closeBlock(&M, H);
    }
    printf("\tAll full blocks processed\n");

    // Padded block - in all cases, all remaining input bytes AND 1bit will fit in this block.
    printf("Processing Remaining Input Bytes:\n");
    // Read the last bytes from the file, knowing that it is less than 64.
    size_t bytesInBlock = fread(M.eight, 1, 64, inFile);
    printf("\tBytes read: %d\n", bytesInBlock);
    // Since this is not a full block, there must be room for at least one more byte, so add the 1bit.
    M.eight[bytesInBlock] = 0x80;
    bytesInBlock++;

    if(bytesInBlock < 56) {
        printf("\tSingle padded block\n");
        // Room for everything here...
        for(int i = bytesInBlock; i < 56; i++) {
            M.eight[i] = 0x00;
        }
        M.sixty_four[7] = htobe64(len);
        closeBlock(&M, H);
    }
    else {
        printf("\tTwo padded blocks\n");
        // Pad the rest of this block with zeroes and add a new fully padded block
        for(int i = bytesInBlock; i < 64; i++){
            M.eight[i] = 0x00;
        }
        closeBlock(&M, H);
        // Now, create the fully padded block and also hash it
        printf("\n\tCreating new block\n");
        createFullyPaddedBlock(&M, len);
        closeBlock(&M, H);
    }

    printf("\n\n\n");
    // Print the hash.
    for (int i = 0; i < 8; i++) {
        printf("%08" PRIx32 "", H[i]);
    }

    printf("\n");

    fclose(inFile);

    return 0;
}
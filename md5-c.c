#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <inttypes.h>
#include <endian.h>

#include "md5-c.h"
#include "sha256.h"

void printHashInfo(HashInfo *h) {

    printf("-------------------------------------------------------------\n");
    printf("                       Input Bytes: %d\n", h->inputBytes);
    printf("                    Reserved Bytes: %d\n", RESERVED_BYTES);
    printf("                Total Bytes Needed: %d\n", h->totalBytesNeeded);
    printf("-------------------------------------------------------------\n");
    printf("                       Full Blocks: %ld\n", h->fullBlocks);
    printf("                     Padded Blocks: %ld\n", h->paddedBlocks);
    printf("        Input Bytes in Full Blocks: %ld\n", h->bytesInFullBlocks);
    printf(" Input Bytes in First Padded Block: %d\n", h->bytesInPaddedBlocks);
    printf("-------------------------------------------------------------\n");
}

void calculateHashInfo(HashInfo *h, long inputBytes) {

    printf("\nCalculating hash info...\n");

    HashInfo hashInfo;

    h->inputBytes = inputBytes;
    h->totalBytesNeeded = inputBytes + RESERVED_BYTES;
    h->fullBlocks = floor((double)(inputBytes) / BLOCK_SIZE);

    h->bytesInFullBlocks = h->fullBlocks * BLOCK_SIZE;
    int remainingInputBytes = h->inputBytes - h->bytesInFullBlocks;
    h->bytesInPaddedBlocks = remainingInputBytes < 0 ? remainingInputBytes + 64: remainingInputBytes;

    h->paddedBlocks = h->bytesInPaddedBlocks < 56 ? 1 : 2;
}

long getFileBytes(FILE *file) {
    // Get the size of the file in bytes
    fseek(file, 0L, SEEK_END);
    long numInputBytes = ftell(file);
    fseek(file, 0L, 0L);

    return numInputBytes;
}

void convertBlockToHostEndianness(BLOCK *block){
    for (int i = 0; i < 16; i++) {
        block->thirty_two[i] = be32toh(block->thirty_two[i]);
    }
}

void closeBlock(BLOCK *M, WORD *H) { 
    convertBlockToHostEndianness(M);
    nextHashSHA256(M->thirty_two, H);
}

void createFullyPaddedBlock(BLOCK *block, uint64_t len) {
    for(int i = 0; i < 56; i++) {
        block->eight[i] = 0x00;
    }
    block->sixty_four[7] = htobe64(len);
}

// typedef union {
//   uint64_t sixfour[8];
//   uint32_t threetwo[16];
//   uint8_t eight[64];
// } BLOCK;

int main(int argc, char *argv[]) {

    printf("--- MD5 Algorithm---\n");
    printf("Block Size: %d Bytes\n\n", BLOCK_SIZE);

    // char fileName[] = "./res/two-pad-block.txt";

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
    // FILE *inFile = fopen(fileName, "rb");

    // Open the file
    // printf("Opening file: %s\n", fileName);

    long fileBytes = getFileBytes(inFile);
    printf("\tBytes in file: %ld\n", fileBytes);

    HashInfo hashInfo;
    calculateHashInfo(&hashInfo, fileBytes);
    printHashInfo(&hashInfo);


    char *buffer; // read in to this buffer from the file.
    // If there are any full blocks, allocate the buffer right now.
    // It will be reallocated for the padded block later.
    if(hashInfo.fullBlocks > 0) buffer = (char*)malloc(64 * sizeof(char));

    printf("\n");
    printf("Processing: %ld full blocks...\n", hashInfo.fullBlocks);

    // Section 5.3.3
    WORD H[] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19  
    };

    BLOCK M;

    uint64_t len = (uint64_t)hashInfo.inputBytes * 8ULL;

    // for each full block...
    for(int i = 0; i < hashInfo.fullBlocks; i++){
        // Read all 64 bytes for this block
        size_t read = fread(M.eight, 1, 64, inFile);
        // convertBlockToHostEndianness(&M);
        // nextHashSHA256(M.thirty_two, H);
        closeBlock(&M, H);
    }
    printf("\tAll full blocks processed\n");

    // Padded block - in all cases, all remaining input bytes AND 1bit will fit in this block.
    printf("Processing Remaining Input Bytes:\n");
    // read the last bytes from the file
    size_t bytesInBlock = fread(M.eight, 1, hashInfo.bytesInPaddedBlocks, inFile);
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
        // convertBlockToHostEndianness(&M);
        // nextHashSHA256(M.thirty_two, H);
        closeBlock(&M, H);
    }
    else {
        printf("\tTwo padded blocks\n");
        // Pad the rest of this block with zeroes and add a new fully padded block
        // First, hash the previous block
        for(int i = bytesInBlock; i < 64; i++){
            M.eight[i] = 0x00;
        }
        convertBlockToHostEndianness(&M);
        nextHashSHA256(M.thirty_two, H);
        // Now, create the fully padded block and also hash it
        printf("\n\tCreating new block\n");
        createFullyPaddedBlock(&M, len);
        // convertBlockToHostEndianness(&M);
        // nextHashSHA256(M.thirty_two, H);
        closeBlock(&M, H);
    }

    printf("\n\n\n");
    // Print the hash.
    for (int i = 0; i < 8; i++) {
        printf("%08" PRIx32 "", H[i]);

        // fprintf(outfile, "%08" PRIx32 "", H[i]);
    }

    printf("\n");

    fclose(inFile);

    return 0;
}
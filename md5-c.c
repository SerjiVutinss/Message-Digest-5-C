#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include <stdlib.h>

const int BLOCK_SIZE = 512 / 8;
const int RESERVED_BYTES = 8;

typedef struct HashInfo {

    // User set members
    long inputBytes;

    // Calculated members
    long totalBytesNeeded;

    // Blocks completely filled with input bytes
    long fullBlocks;
    int paddedBlocks;

    int bytesNotInFullBlocks;


    long bytesInFullBlocks;
    int bytesInLastBlock;

    int bytesInPaddedBlocks;

    int hasFinalEmptyBlock;

} HashInfo;

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
    h->fullBlocks = floor((double)(inputBytes + RESERVED_BYTES) / BLOCK_SIZE);

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

typedef union {
  uint64_t sixfour[8];
  uint32_t threetwo[16];
  uint8_t eight[64];
} BLOCK;

int main(int argc, char *argv) {

    printf("--- MD5 Algorithm---\n");
    printf("Block Size: %d Bytes\n\n", BLOCK_SIZE);

    char fileName[] = "./res/one-pad-block.txt";

    // Open the file
    printf("Opening file: %s\n", fileName);
    FILE *inFile = fopen(fileName, "rb");

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
    printf("Processing: %ld full blocks\n", hashInfo.fullBlocks);

    BLOCK M;
    // for each full block...
    for(int i = 0; i < hashInfo.fullBlocks; i++){
        // Read all 64 bytes for this block
        size_t read = fread(M.eight, 1, 64, inFile);
        printf("Read %ld\n", read);
        for(int k = 0; k < 64; k++) {
            printf("%c", M.eight[k]);

            // !!hash the block here!!
        }
        printf("\n");
    }

    // Padded blocks...
    // In both cases, all remaining input bytes will fit in the first block.
    // Reallocate the buffer to fit the remaining bytes
    buffer = (char*)realloc(buffer, hashInfo.bytesInPaddedBlocks * sizeof(char));
    printf("Processing Remaining Input Bytes\n");
    // read the last bytes from the file
    fread(M.eight, hashInfo.bytesInPaddedBlocks, 1, inFile);
    for(int i = 0; i < hashInfo.bytesInPaddedBlocks; i++) {
        printf("%c", M.eight[i]);
    }
    printf("\n");

    printf("Adding 1bit\n");
    printf("%" PRIx8 "\n", 0x80);

    uint64_t len = hashInfo.inputBytes * 8;

    printf("Bytes so far in first padded block %d\n", hashInfo.bytesInPaddedBlocks + 1);
    if(hashInfo.paddedBlocks == 1) {
        // Pad out this block normally, adding the length
        int bytesToPad = (64 - 8) - (hashInfo.bytesInPaddedBlocks + 1);
        printf("Padding current block with %d bytes\n", bytesToPad);
        for(int i = 0; i < bytesToPad; i++) {
            printf("%" PRIx8, 0x00);
        }
        printf("\nAdding length %d\n", len);
        printf("%" PRIx64, len);
    } else {
        // Need to just pad the remaining bytes in this block with zero bytes
        int bytesToPad = (64 - (hashInfo.bytesInPaddedBlocks + 1));
        for(int i = 0; i < bytesToPad; i++) {
            printf("%" PRIx8, 0x00);
        }
    }
    printf("\n");

    // !!hash the block here!!

    // finally, check if a last block is needed and add if it is
    if(hashInfo.paddedBlocks == 2) {
        printf("Creating all padding block\n");
        for(int i = 0; i < 56; i++) {
            printf("%" PRIx8, 0x00);
        }
        printf("%" PRIX64, len);
    }

    printf("\n");
    printf("\nDone\n");

    fclose(inFile);

    return 0;
}
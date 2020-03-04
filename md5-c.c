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

void convertBlockToHostEndianness(BLOCK *block){
    for (int i = 0; i < 16; i++) {
        block->thirty_two[i] = be32toh(block->thirty_two[i]);
    }
}

// typedef union {
//   uint64_t sixfour[8];
//   uint32_t threetwo[16];
//   uint8_t eight[64];
// } BLOCK;

int main(int argc, char *argv) {

    printf("--- MD5 Algorithm---\n");
    printf("Block Size: %d Bytes\n\n", BLOCK_SIZE);

    char fileName[] = "./res/no-pad-block.txt";

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

    // Section 5.3.3
    WORD H[] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19  
    };

    BLOCK M;
    // for each full block...
    for(int i = 0; i < hashInfo.fullBlocks; i++){
        // Read all 64 bytes for this block
        size_t read = fread(M.eight, 1, 64, inFile);
        printf("Read %ld\n", read);
        for(int k = 0; k < 64; k++) {
            printf("%c", M.eight[k]);

            // !!hash the block here!!
            nextHashSHA256(M.thirty_two, H);
        }
        printf("\n");
    }

    // All FULL BLOCKS Complete...

    // ADD FIRST PADDED BLOCK NOW

    // In both cases, all remaining input bytes will fit in the first block.
    // Reallocate the buffer to fit the remaining bytes
    buffer = (char*)realloc(buffer, hashInfo.bytesInPaddedBlocks * sizeof(char));
    printf("Processing Remaining Input Bytes\n");
    // read the last bytes from the file
    size_t bytesRead = fread(M.eight, 1, hashInfo.bytesInPaddedBlocks, inFile);
    printf("BYTES READ IN PADDED BLOCK: %d\n", bytesRead);
    int count;
    for(count = 0; count < hashInfo.bytesInPaddedBlocks; count++) {
        printf("%c", M.eight[count]);
    }
    printf("\n%d bytes added in padded block\n", count);

    printf("\n\nPOSITION OF 1BIT: %d\n", count);
    printf("Adding 1bit\n");
    printf("%" PRIx8 "\n", 0x80);
    M.eight[count] = 0x80;
    count++;

    printf("\n\nPOSITION OF FIRST PADDED BYTE: %d\n", count);
    printf("\n");

    uint64_t len = (uint64_t)hashInfo.inputBytes * 8ULL;

    printf("Bytes so far in first padded block %d\n", hashInfo.bytesInPaddedBlocks + 1);
    if(hashInfo.paddedBlocks == 1) {
        // Pad out this block normally, adding the length
        int bytesToPad = (64 - 8) - (hashInfo.bytesInPaddedBlocks + 1);
        printf("Padding current block with %d bytes\n", bytesToPad);
        for(int i = count; i < 56; i++) {
            printf("\nPadding position %d with %" PRIx8, i,  0x00);
            M.eight[i] = 0x00;
        }


        printf("\nAdding length %d\n", len);
        // Now we should be at last 64bit...
        printf("%" PRIx64, len);
        M.sixty_four[7] = htobe64(len);

    } else {
        // Need to just pad the remaining bytes in this block with zero bytes
        int bytesToPad = (64 - (hashInfo.bytesInPaddedBlocks + 1));
        for(int i = 0; i < bytesToPad; i++) {
            printf("%" PRIx8, 0x00);
        M.sixty_four[7] = htobe64(len);
        }
    }

// Convert to host endianess, word-size-wise.
    convertBlockToHostEndianness(&M);
    // for (int i = 0; i < 16; i++)
    //     M.thirty_two[i] = be32toh(M.thirty_two[i]);

    printf("\n");

    // !!hash the block here!!
    nextHashSHA256(M.thirty_two, H);

    // finally, check if a last block is needed and add if it is
    if(hashInfo.paddedBlocks == 2) {
        printf("Creating all padding block\n");
        for(int i = 0; i < 56; i++) {
            printf("%" PRIx8, 0x00);
        }
        printf("%" PRIX64, len);
        nextHashSHA256(M.thirty_two, H);
    }

    printf("\n\n\n");
    // Print the hash.
    for (int i = 0; i < 8; i++) {
        printf("%08" PRIx32 "", H[i]);

        // fprintf(outfile, "%08" PRIx32 "", H[i]);
    }

    printf("\n");
    printf("\nDone\n");

    fclose(inFile);

    return 0;
}
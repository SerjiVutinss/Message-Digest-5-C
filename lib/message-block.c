#include "message-block.h"

void convertBlockToHostEndianness(MessageBlock *block){
    for (int i = 0; i < 16; i++) {
        block->thirty_two[i] = be32toh(block->thirty_two[i]);
    }
}

void createFullyPaddedBlock(MessageBlock *block, uint64_t len) {
    for(int i = 0; i < 56; i++) {
        block->eight[i] = 0x00;
    }
    block->sixty_four[7] = htobe64(len);
}

// void closeBlock(Block *M, WORD *ALG_WORD, int alg) { 
    
//     convertBlockToHostEndianness(M);

//     for (int i = 0; i < 16; i++) {
//       printf("M: %08" PRIx32 "\n", M->thirty_two[i]);
//     }

//     if(alg == 0) nextHashSHA256(M->thirty_two, ALG_WORD);
//     else nextHashMD5(M, ALG_WORD);
// }

// void printReverseEndian(unsigned n) {
//   printf("%02x%02x%02x%02x", n & 0xff, (n >> 8) & 0xff, (n >> 16) & 0xff, n >> 24);
// }




    // printf("\n");
    // printf("Processing: %ld full blocks...\n", hashInfo.fullBlocks);

    // // Section 5.3.3
    // WORD H_SHA[] = {
    //     0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    //     0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19  
    // };

    // WORD H_MD5[] = { 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476 };

    // WORD *ALG_WORD;
    // if(algorithm == 0) ALG_WORD = H_SHA;
    // else ALG_WORD = H_MD5;

    // Block M;

    // uint64_t len = (uint64_t)hashInfo.inputBytes * 8ULL;

    // // Process the full blocks
    // for(int i = 0; i < hashInfo.fullBlocks; i++){
    //     // Read all 64 bytes for this block
    //     size_t read = fread(M.eight, 1, 64, inFile);
    //     // closeBlock(&M, H_SHA);
    //     closeBlock(&M, ALG_WORD, algorithm);
    // }
    // printf("\tAll full blocks processed\n");

    // // Padded block - in all cases, all remaining input bytes AND 1bit will fit in this block.
    // printf("Processing Remaining Input Bytes:\n");
    // // Read the last bytes from the file, knowing that it is less than 64.
    // size_t bytesInBlock = fread(M.eight, 1, 64, inFile);
    // printf("\tBytes read: %d\n", bytesInBlock);
    // // Since this is not a full block, there must be room for at least one more byte, so add the 1bit.
    // M.eight[bytesInBlock] = 0x80;
    // bytesInBlock++;

    // if(bytesInBlock < 56) {
    //     printf("\tSingle padded block\n");
    //     // Room for everything here...
    //     for(int i = bytesInBlock; i < 56; i++) {
    //         M.eight[i] = 0x00;
    //     }
    //     M.sixty_four[7] = htobe64(len);
    //     // closeBlock(&M, H_SHA);
    //     closeBlock(&M, ALG_WORD, algorithm);
    // }
    // else {
    //     printf("\tTwo padded blocks\n");
    //     // Pad the rest of this block with zeroes and add a new fully padded block
    //     for(int i = bytesInBlock; i < 64; i++){
    //         M.eight[i] = 0x00;
    //     }
    //     // closeBlock(&M, H_SHA);
    //     closeBlock(&M, ALG_WORD, algorithm);

    //     // Now, create the fully padded block and also hash it
    //     printf("\n\tCreating new block\n");
    //     createFullyPaddedBlock(&M, len);
    //     // closeBlock(&M, H_SHA);
    //     closeBlock(&M, ALG_WORD, algorithm);
    // }
    // // printf("Length: %08" PRIx64 "\n", len);

    // printf("\n\n\n");
    
    // printf("OUTPUT:\n");
    // if(algorithm == 0) {
    //     for (int i = 0; i < 8; i++) {
    //         printf("%08" PRIx32 "", ALG_WORD[i]);
    //     }
    // }
    // else {
    //     // printf("\n");
    //     // for (int i = 0; i < 4; i++) {
    //     //     printReverseEndian(ALG_WORD[i]);
    //     // }


    //     for (int i = 0; i < 4; ++i)
    //     printf("%02x%02x%02x%02x", (ALG_WORD[i] >> 0) & 0xFF, (ALG_WORD[i] >> 8) & 0xFF, (ALG_WORD[i] >> 16) & 0xFF, (ALG_WORD[i] >> 24) & 0xFF);

    // }


    // printf("\n");

    // fclose(inFile);
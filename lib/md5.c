#include "md5.h"

WORD *startMD5Hash(HashOptions hashOptions)
{
    printf("STARTING MD5 HASH\n");

    MessageBlock M;                // Declare a message block which file bytes will be read into.
    uint64_t numBits = 0;          // Keep track of the number of bits read from the file.
    enum HashStatus status = READ; // Current status of the algorithm.

    int (*processFunctionPtr)(MessageBlock * M, size_t numBytesRead, size_t * numBits, enum HashStatus * status);
    processFunctionPtr = &processNextMD5Block;

    WORD *completedHash;
    if (hashOptions.isFile == true)
    {

        // While there are still bytes in the file, keep processing blocks as needed.
        while (processFileBlock(&M, hashOptions.file, &numBits, &status, processFunctionPtr))
        {
            // Hash the current block, passing in both the Block and the MD5 initialisers.
            nextMD5Hash(M.thirty_two, H_MD5);
        }

        fclose(hashOptions.file);
    }
    else if (hashOptions.isString == true)
    {
        // While there are still bytes in the file, keep processing blocks as needed.
        while (processStringBlock(&M, hashOptions.string, &numBits, &status, processFunctionPtr))
        {
            // Hash the current block, passing in both the Block and the MD5 initialisers.
            nextMD5Hash(M.thirty_two, H_MD5);
        }
    }

    return H_MD5;
}

/**
 * Read and process a message block from the input file.
 * 
 * Return 1 if there are still bytes in the file, otherwise return 0.
 **/
int processNextMD5Block(MessageBlock *M, size_t numBytesRead, size_t *numBits, enum HashStatus *status)
{
    // If the Status FINISHED was passed in, return 0 from this function.
    if (*status == FINISHED)
        return 0;

    // If the Status PAD_ZEROES was passed in, this block should be fully padded
    // and the length of the message appended.
    if (*status == PAD_ZEROES)
    {
        // Pad with zeroes.
        for (int i = 0; i < 57; i++)
            M->eight[i] = 0;
        // Append the total message length.
        M->sixty_four[7] = *numBits;
        // And set the Status to FINISHED.
        *status = FINISHED;
        return 1;
    }

    // A full block was read, so nothing further needs to be done with this block.
    if (numBytesRead == 64)
        return 1;
    // In this case, append the one-bit, pad with zeroes and then append the length.
    else if (numBytesRead < 56)
    {
        // Append the one-bit
        M->eight[numBytesRead] = 0x80;
        // Pad with correct number of zeroes...
        for (int i = numBytesRead + 1; i < 56; i++)
            M->eight[i] = 0x00;
        // ...and append the length
        M->sixty_four[7] = *numBits;
        // And set the Status to FINISHED.
        *status = FINISHED;
        return 1;
    }
    else
    {
        // In this case, we append the one-bit and pad with zeroes, but since
        // there is not ebough room to append the length, Status is set to PAD_ZEROES
        // to indicate that a new, fully-padded block should be created.

        // Append the one-bit
        M->eight[numBytesRead] = 0x80;
        // Pad the rest of the block with zeroes
        for (int i = numBytesRead + 1; i < 64; i++)
            M->eight[i] = 0;
        // And set Status
        *status = PAD_ZEROES;
        return 1;
    }
}

void nextMD5Hash(WORD *M, WORD *H)
{

    // printf("New Block:\n");

    // // Print out this block
    // for (int i = 0; i < 16; i++)
    // {
    //     printf("\tM: %08" PRIx32 "\n", M->thirty_two[i]);
    // }

    uint32_t copyH[4];
    uint32_t A = H[0];
    uint32_t B = H[1];
    uint32_t C = H[2];
    uint32_t D = H[3];

    FF(&H[0], H[1], H[2], H[3], M[0], S[0], K_MD5[0]); /* 1 */
    FF(&H[3], H[0], H[1], H[2], M[1], S[1], K_MD5[1]); /* 2 */
    FF(&H[2], H[3], H[0], H[1], M[2], S[2], K_MD5[2]); /* 3 */
    FF(&H[1], H[2], H[3], H[0], M[3], S[3], K_MD5[3]); /* 4 */

    FF(&H[0], H[1], H[2], H[3], M[4], S[4], K_MD5[4]); /* 5 */
    FF(&H[3], H[0], H[1], H[2], M[5], S[5], K_MD5[5]); /* 6 */
    FF(&H[2], H[3], H[0], H[1], M[6], S[6], K_MD5[6]); /* 7 */
    FF(&H[1], H[2], H[3], H[0], M[7], S[7], K_MD5[7]); /* 8 */

    FF(&H[0], H[1], H[2], H[3], M[8], S[8], K_MD5[8]);    /* 9 */
    FF(&H[3], H[0], H[1], H[2], M[9], S[9], K_MD5[9]);    /* 10 */
    FF(&H[2], H[3], H[0], H[1], M[10], S[10], K_MD5[10]); /* 11 */
    FF(&H[1], H[2], H[3], H[0], M[11], S[11], K_MD5[11]); /* 12 */

    FF(&H[0], H[1], H[2], H[3], M[12], S[12], K_MD5[12]); /* 13 */
    FF(&H[3], H[0], H[1], H[2], M[13], S[13], K_MD5[13]); /* 14 */
    FF(&H[2], H[3], H[0], H[1], M[14], S[14], K_MD5[14]); /* 15 */
    FF(&H[1], H[2], H[3], H[0], M[15], S[15], K_MD5[15]); /* 16 */

    GG(&H[0], H[1], H[2], H[3], M[1], S[16], K_MD5[16]);  /* 17 */
    GG(&H[3], H[0], H[1], H[2], M[6], S[17], K_MD5[17]);  /* 18 */
    GG(&H[2], H[3], H[0], H[1], M[11], S[18], K_MD5[18]); /* 19 */
    GG(&H[1], H[2], H[3], H[0], M[0], S[19], K_MD5[19]);  /* 20 */
    GG(&H[0], H[1], H[2], H[3], M[5], S[20], K_MD5[20]);  /* 21 */
    GG(&H[3], H[0], H[1], H[2], M[10], S[21], K_MD5[21]); /* 22 */
    GG(&H[2], H[3], H[0], H[1], M[15], S[22], K_MD5[22]); /* 23 */
    GG(&H[1], H[2], H[3], H[0], M[4], S[23], K_MD5[23]);  /* 24 */
    GG(&H[0], H[1], H[2], H[3], M[9], S[24], K_MD5[24]);  /* 25 */
    GG(&H[3], H[0], H[1], H[2], M[14], S[25], K_MD5[25]); /* 26 */
    GG(&H[2], H[3], H[0], H[1], M[3], S[26], K_MD5[26]);  /* 27 */
    GG(&H[1], H[2], H[3], H[0], M[8], S[27], K_MD5[27]);  /* 28 */
    GG(&H[0], H[1], H[2], H[3], M[13], S[28], K_MD5[28]); /* 29 */
    GG(&H[3], H[0], H[1], H[2], M[2], S[29], K_MD5[29]);  /* 30 */
    GG(&H[2], H[3], H[0], H[1], M[7], S[30], K_MD5[30]);  /* 31 */
    GG(&H[1], H[2], H[3], H[0], M[12], S[31], K_MD5[31]); /* 32 */

    HH(&H[0], H[1], H[2], H[3], M[5], S[32], K_MD5[32]);  /* 33 */
    HH(&H[3], H[0], H[1], H[2], M[8], S[33], K_MD5[33]);  /* 34 */
    HH(&H[2], H[3], H[0], H[1], M[11], S[34], K_MD5[34]); /* 35 */
    HH(&H[1], H[2], H[3], H[0], M[14], S[35], K_MD5[35]); /* 36 */
    HH(&H[0], H[1], H[2], H[3], M[1], S[36], K_MD5[36]);  /* 37 */
    HH(&H[3], H[0], H[1], H[2], M[4], S[37], K_MD5[37]);  /* 38 */
    HH(&H[2], H[3], H[0], H[1], M[7], S[38], K_MD5[38]);  /* 39 */
    HH(&H[1], H[2], H[3], H[0], M[10], S[39], K_MD5[39]); /* 40 */
    HH(&H[0], H[1], H[2], H[3], M[13], S[40], K_MD5[40]); /* 41 */
    HH(&H[3], H[0], H[1], H[2], M[0], S[41], K_MD5[41]);  /* 42 */
    HH(&H[2], H[3], H[0], H[1], M[3], S[42], K_MD5[42]);  /* 43 */
    HH(&H[1], H[2], H[3], H[0], M[6], S[43], K_MD5[43]);  /* 44 */
    HH(&H[0], H[1], H[2], H[3], M[9], S[44], K_MD5[44]);  /* 45 */
    HH(&H[3], H[0], H[1], H[2], M[12], S[45], K_MD5[45]); /* 46 */
    HH(&H[2], H[3], H[0], H[1], M[15], S[46], K_MD5[46]); /* 47 */
    HH(&H[1], H[2], H[3], H[0], M[2], S[47], K_MD5[47]);  /* 48 */

    II(&H[0], H[1], H[2], H[3], M[0], S[48], K_MD5[48]);  /* 49 */
    II(&H[3], H[0], H[1], H[2], M[7], S[49], K_MD5[49]);  /* 50 */
    II(&H[2], H[3], H[0], H[1], M[14], S[50], K_MD5[50]); /* 51 */
    II(&H[1], H[2], H[3], H[0], M[5], S[51], K_MD5[51]);  /* 52 */
    II(&H[0], H[1], H[2], H[3], M[12], S[52], K_MD5[52]); /* 53 */
    II(&H[3], H[0], H[1], H[2], M[3], S[53], K_MD5[53]);  /* 54 */
    II(&H[2], H[3], H[0], H[1], M[10], S[54], K_MD5[54]); /* 55 */
    II(&H[1], H[2], H[3], H[0], M[1], S[55], K_MD5[55]);  /* 56 */
    II(&H[0], H[1], H[2], H[3], M[8], S[56], K_MD5[56]);  /* 57 */
    II(&H[3], H[0], H[1], H[2], M[15], S[57], K_MD5[57]); /* 58 */
    II(&H[2], H[3], H[0], H[1], M[6], S[58], K_MD5[58]);  /* 59 */
    II(&H[1], H[2], H[3], H[0], M[13], S[59], K_MD5[59]); /* 60 */
    II(&H[0], H[1], H[2], H[3], M[4], S[60], K_MD5[60]);  /* 61 */
    II(&H[3], H[0], H[1], H[2], M[11], S[61], K_MD5[61]); /* 62 */
    II(&H[2], H[3], H[0], H[1], M[2], S[62], K_MD5[62]);  /* 63 */
    II(&H[1], H[2], H[3], H[0], M[9], S[63], K_MD5[63]);  /* 64 */

    // FF(&H[0], H[1], H[2], H[3], M->thirty_two[0], S[0], K_MD5[0]); /* 1 */
    // FF(&H[3], H[0], H[1], H[2], M->thirty_two[1], S[1], K_MD5[1]); /* 2 */
    // FF(&H[2], H[3], H[0], H[1], M->thirty_two[2], S[2], K_MD5[2]); /* 3 */
    // FF(&H[1], H[2], H[3], H[0], M->thirty_two[3], S[3], K_MD5[3]); /* 4 */

    // FF(&H[0], H[1], H[2], H[3], M->thirty_two[4], S[4], K_MD5[4]); /* 5 */
    // FF(&H[3], H[0], H[1], H[2], M->thirty_two[5], S[5], K_MD5[5]); /* 6 */
    // FF(&H[2], H[3], H[0], H[1], M->thirty_two[6], S[6], K_MD5[6]); /* 7 */
    // FF(&H[1], H[2], H[3], H[0], M->thirty_two[7], S[7], K_MD5[7]); /* 8 */

    // FF(&H[0], H[1], H[2], H[3], M->thirty_two[8], S[8], K_MD5[8]);    /* 9 */
    // FF(&H[3], H[0], H[1], H[2], M->thirty_two[9], S[9], K_MD5[9]);    /* 10 */
    // FF(&H[2], H[3], H[0], H[1], M->thirty_two[10], S[10], K_MD5[10]); /* 11 */
    // FF(&H[1], H[2], H[3], H[0], M->thirty_two[11], S[11], K_MD5[11]); /* 12 */

    // FF(&H[0], H[1], H[2], H[3], M->thirty_two[12], S[12], K_MD5[12]); /* 13 */
    // FF(&H[3], H[0], H[1], H[2], M->thirty_two[13], S[13], K_MD5[13]); /* 14 */
    // FF(&H[2], H[3], H[0], H[1], M->thirty_two[14], S[14], K_MD5[14]); /* 15 */
    // FF(&H[1], H[2], H[3], H[0], M->thirty_two[15], S[15], K_MD5[15]); /* 16 */

    // GG(&H[0], H[1], H[2], H[3], M->thirty_two[1], S[16], K_MD5[16]);  /* 17 */
    // GG(&H[3], H[0], H[1], H[2], M->thirty_two[6], S[17], K_MD5[17]);  /* 18 */
    // GG(&H[2], H[3], H[0], H[1], M->thirty_two[11], S[18], K_MD5[18]); /* 19 */
    // GG(&H[1], H[2], H[3], H[0], M->thirty_two[0], S[19], K_MD5[19]);  /* 20 */
    // GG(&H[0], H[1], H[2], H[3], M->thirty_two[5], S[20], K_MD5[20]);  /* 21 */
    // GG(&H[3], H[0], H[1], H[2], M->thirty_two[10], S[21], K_MD5[21]); /* 22 */
    // GG(&H[2], H[3], H[0], H[1], M->thirty_two[15], S[22], K_MD5[22]); /* 23 */
    // GG(&H[1], H[2], H[3], H[0], M->thirty_two[4], S[23], K_MD5[23]);  /* 24 */
    // GG(&H[0], H[1], H[2], H[3], M->thirty_two[9], S[24], K_MD5[24]);  /* 25 */
    // GG(&H[3], H[0], H[1], H[2], M->thirty_two[14], S[25], K_MD5[25]); /* 26 */
    // GG(&H[2], H[3], H[0], H[1], M->thirty_two[3], S[26], K_MD5[26]);  /* 27 */
    // GG(&H[1], H[2], H[3], H[0], M->thirty_two[8], S[27], K_MD5[27]);  /* 28 */
    // GG(&H[0], H[1], H[2], H[3], M->thirty_two[13], S[28], K_MD5[28]); /* 29 */
    // GG(&H[3], H[0], H[1], H[2], M->thirty_two[2], S[29], K_MD5[29]);  /* 30 */
    // GG(&H[2], H[3], H[0], H[1], M->thirty_two[7], S[30], K_MD5[30]);  /* 31 */
    // GG(&H[1], H[2], H[3], H[0], M->thirty_two[12], S[31], K_MD5[31]); /* 32 */

    // HH(&H[0], H[1], H[2], H[3], M->thirty_two[5], S[32], K_MD5[32]);  /* 33 */
    // HH(&H[3], H[0], H[1], H[2], M->thirty_two[8], S[33], K_MD5[33]);  /* 34 */
    // HH(&H[2], H[3], H[0], H[1], M->thirty_two[11], S[34], K_MD5[34]); /* 35 */
    // HH(&H[1], H[2], H[3], H[0], M->thirty_two[14], S[35], K_MD5[35]); /* 36 */
    // HH(&H[0], H[1], H[2], H[3], M->thirty_two[1], S[36], K_MD5[36]);  /* 37 */
    // HH(&H[3], H[0], H[1], H[2], M->thirty_two[4], S[37], K_MD5[37]);  /* 38 */
    // HH(&H[2], H[3], H[0], H[1], M->thirty_two[7], S[38], K_MD5[38]);  /* 39 */
    // HH(&H[1], H[2], H[3], H[0], M->thirty_two[10], S[39], K_MD5[39]); /* 40 */
    // HH(&H[0], H[1], H[2], H[3], M->thirty_two[13], S[40], K_MD5[40]); /* 41 */
    // HH(&H[3], H[0], H[1], H[2], M->thirty_two[0], S[41], K_MD5[41]);  /* 42 */
    // HH(&H[2], H[3], H[0], H[1], M->thirty_two[3], S[42], K_MD5[42]);  /* 43 */
    // HH(&H[1], H[2], H[3], H[0], M->thirty_two[6], S[43], K_MD5[43]);  /* 44 */
    // HH(&H[0], H[1], H[2], H[3], M->thirty_two[9], S[44], K_MD5[44]);  /* 45 */
    // HH(&H[3], H[0], H[1], H[2], M->thirty_two[12], S[45], K_MD5[45]); /* 46 */
    // HH(&H[2], H[3], H[0], H[1], M->thirty_two[15], S[46], K_MD5[46]); /* 47 */
    // HH(&H[1], H[2], H[3], H[0], M->thirty_two[2], S[47], K_MD5[47]);  /* 48 */

    // II(&H[0], H[1], H[2], H[3], M->thirty_two[0], S[48], K_MD5[48]);  /* 49 */
    // II(&H[3], H[0], H[1], H[2], M->thirty_two[7], S[49], K_MD5[49]);  /* 50 */
    // II(&H[2], H[3], H[0], H[1], M->thirty_two[14], S[50], K_MD5[50]); /* 51 */
    // II(&H[1], H[2], H[3], H[0], M->thirty_two[5], S[51], K_MD5[51]);  /* 52 */
    // II(&H[0], H[1], H[2], H[3], M->thirty_two[12], S[52], K_MD5[52]); /* 53 */
    // II(&H[3], H[0], H[1], H[2], M->thirty_two[3], S[53], K_MD5[53]);  /* 54 */
    // II(&H[2], H[3], H[0], H[1], M->thirty_two[10], S[54], K_MD5[54]); /* 55 */
    // II(&H[1], H[2], H[3], H[0], M->thirty_two[1], S[55], K_MD5[55]);  /* 56 */
    // II(&H[0], H[1], H[2], H[3], M->thirty_two[8], S[56], K_MD5[56]);  /* 57 */
    // II(&H[3], H[0], H[1], H[2], M->thirty_two[15], S[57], K_MD5[57]); /* 58 */
    // II(&H[2], H[3], H[0], H[1], M->thirty_two[6], S[58], K_MD5[58]);  /* 59 */
    // II(&H[1], H[2], H[3], H[0], M->thirty_two[13], S[59], K_MD5[59]); /* 60 */
    // II(&H[0], H[1], H[2], H[3], M->thirty_two[4], S[60], K_MD5[60]);  /* 61 */
    // II(&H[3], H[0], H[1], H[2], M->thirty_two[11], S[61], K_MD5[61]); /* 62 */
    // II(&H[2], H[3], H[0], H[1], M->thirty_two[2], S[62], K_MD5[62]);  /* 63 */
    // II(&H[1], H[2], H[3], H[0], M->thirty_two[9], S[63], K_MD5[63]);  /* 64 */

    H[0] += A;
    H[1] += B;
    H[2] += C;
    H[3] += D;
}

// WORD *startMD5StringHash(char *inputString)
// {

//     MessageBlock M;                // Declare a message block which file bytes will be read into.
//     uint64_t numBits = 0;          // Keep track of the number of bits read from the file.
//     enum HashStatus status = READ; // Current status of the algorithm.

//     int (*processFunctionPtr)(MessageBlock * M, size_t numBytesRead, size_t * numBits, enum HashStatus * status);
//     processFunctionPtr = &processNextMD5Block;

//     // While there are still bytes in the file, keep processing blocks as needed.
//     while (processStringBlock(&M, inputString, &numBits, &status, processFunctionPtr))
//     {
//         // Hash the current block, passing in both the Block and the MD5 initialisers.
//         nextMD5Hash(M.thirty_two, H_MD5);
//     }

//     return H_MD5;
// }

// WORD *startMD5FileHash(FILE *inFile)
// {

//     MessageBlock M;                // Declare a message block which file bytes will be read into.
//     uint64_t numBits = 0;          // Keep track of the number of bits read from the file.
//     enum HashStatus status = READ; // Current status of the algorithm.

//     int (*processFunctionPtr)(MessageBlock * M, size_t numBytesRead, size_t * numBits, enum HashStatus * status);
//     processFunctionPtr = &processNextMD5Block;

//     // While there are still bytes in the file, keep processing blocks as needed.
//     while (processFileBlock(&M, inFile, &numBits, &status, processFunctionPtr))
//     {
//         // Hash the current block, passing in both the Block and the MD5 initialisers.
//         nextMD5Hash(M.thirty_two, H_MD5);
//     }

//     fclose(inFile);

//     return H_MD5;
// }
#include "common-alg.h"

size_t getBytesFromString(char buffer[], char *inStr, int start, int length)
{
    memcpy(buffer, &inStr[start], length);

    for (int i = 0; i < 64; i++)
    {
        if (buffer[i] == '\0')
        {
            // reached the end
            return i;
        }
    }
    return 64;
}

int processStringBlock(MessageBlock *M, char *buffer, size_t *numBits, enum HashStatus *status,
                       int (*processNextBlock)(MessageBlock *M, size_t numBytesRead, size_t *numBits, enum HashStatus *status))
{
    size_t bytesRead = (*numBits / 8ULL);
    size_t newBytesRead = getBytesFromString(M->eight, buffer, bytesRead, 64);
    // *numBits += newBytesRead * 8;
    *numBits += (8ULL * ((uint64_t)newBytesRead));

    return (*processNextBlock)(M, newBytesRead, numBits, status);
}

int processFileBlock(MessageBlock *M, FILE *infile, size_t *numBits, enum HashStatus *status,
                     int (*processNextBlock)(MessageBlock *M, size_t numBytesRead, size_t *numBits, enum HashStatus *status))
{
    // Attempt to read 64 (512 bits) bytes from the file into the Block.
    size_t numFileBytesRead = fread(&M->eight, 1, 64, infile);
    // Increment the total bits read by the number of recently read bits.
    // *numBits += numFileBytesRead * 8;
    *numBits += (8ULL * ((uint64_t)numFileBytesRead));

    return (*processNextBlock)(M, numFileBytesRead, numBits, status);
}
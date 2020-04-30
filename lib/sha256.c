/**
 * Modified code based on Ian McLoughlin's (https://github.com/ianmcloughlin) own version
 *  Original code can be found at https://github.com/ianmcloughlin/sha256/blob/master/sha256.c
 * 
 * The Secure Hash Algorithm 256-bit version - https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
 **/

#include "sha256.h"

bool printSHA256 = false;
int sha256Blocks = 0;

WORD *startSHA256Hash(HashOptions hashOptions)
{

  printSHA256 = hashOptions.isVerbose;

  printf("\nStarting SHA256 hash...\n");

  MessageBlock M;                // Declare a message block which file bytes will be read into.
  uint64_t numBits = 0;          // Keep track of the number of bits read from the file.
  enum HashStatus status = READ; // Current status of the algorithm.

  int (*processFunctionPtr)(MessageBlock * M, size_t numBytesRead, size_t * numBits, enum HashStatus * status);
  processFunctionPtr = &processNextSHA256Block;

  WORD *completedHash;
  if (hashOptions.isFile == true)
  {

    // While there are still bytes in the file, keep processing blocks as needed.
    while (processFileBlock(&M, hashOptions.file, &numBits, &status, processFunctionPtr))
    {
      sha256Blocks += 1;
      // Hash the current block, passing in both the Block and the MD5 initialisers.
      nextSHA256Hash(M.thirty_two, H_SHA256);
    }

    fclose(hashOptions.file);
  }
  else if (hashOptions.isString == true)
  {
    // While there are still bytes in the file, keep processing blocks as needed.
    while (processStringBlock(&M, hashOptions.string, &numBits, &status, processFunctionPtr))
    {
      // Hash the current block, passing in both the Block and the MD5 initialisers.
      nextSHA256Hash(M.thirty_two, H_SHA256);
    }
  }

  return H_SHA256;
}

// Section 5.1.1 - message input from infile.
int processNextSHA256Block(MessageBlock *M, size_t numBytesRead, uint64_t *numBits, HashStatus *status)
{

  int i;
  if (*status == FINISHED)
    return 0;
  if (*status == PAD_ZEROES)
  {
    // We need an all-padding block without the 1 bit.
    for (i = 0; i < 56; i++)
      M->eight[i] = 0x00;
    M->sixty_four[7] = htobe64(*numBits);
    *status = FINISHED;
  }

  else
  {

    if (numBytesRead < 56)
    {
      // We can put all padding in this block.
      M->eight[numBytesRead] = 0x80;
      for (i = numBytesRead + 1; i < 56; i++)
        M->eight[i] = 0x00;
      M->sixty_four[7] = htobe64(*numBits);
      *status = FINISHED;
    }
    else if (numBytesRead < 64)
    {
      // Otherwise we have read between 56 (incl) and 64 (excl) bytes.
      M->eight[numBytesRead] = 0x80;
      for (int i = numBytesRead + 1; i < 64; i++)
        M->eight[i] = 0x00;
      *status = PAD_ZEROES;
    }
  }

  // Convert to host endianess, word-size-wise.
  for (i = 0; i < 16; i++)
    M->thirty_two[i] = be32toh(M->thirty_two[i]);

  return 1;
}

// Section 6.2.2
void nextSHA256Hash(WORD *M, WORD *H)
{

  WORD W[64];
  WORD a, b, c, d, e, f, g, h, T1, T2;
  int t;

  printf("\nBlock %d: ", sha256Blocks);
  if (printSHA256)
  {
    // Print out this block
    for (int i = 0; i < 16; i++)
    {
      printf("%08" PRIx32 " ", M[i]);
    }
    printf("\n");
  }

  // {
  //   for (int i = 0; i < 16; i++)
  //   {
  //     W[i] = M[i];
  //     printf("M: %08" PRIx32 "\n", W[i]);
  //   }
  // }

  for (t = 0; t < 16; t++)
  {
    W[t] = M[t];
  }

  for (t = 16; t < 64; t++)
  {
    W[t] = sig1(W[t - 2]) + W[t - 7] + sig0(W[t - 15]) + W[t - 16];
  }

  a = H[0];
  b = H[1];
  c = H[2];
  d = H[3];
  e = H[4];
  f = H[5];
  g = H[6];
  h = H[7];

  for (t = 0; t < 64; t++)
  {
    T1 = h + Sig1(e) + Ch(e, f, g) + K_SHA256[t] + W[t];
    T2 = Sig0(a) + Maj(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d + T1;
    d = c;
    c = b;
    b = a;
    a = T1 + T2;
  }

  H[0] += a;
  H[1] += b;
  H[2] += c;
  H[3] += d;
  H[4] += e;
  H[5] += f;
  H[6] += g;
  H[7] += h;
}
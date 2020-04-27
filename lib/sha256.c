/**
 * Modified code based on Ian McLoughlin's (https://github.com/ianmcloughlin) own version
 *  Original code can be found at https://github.com/ianmcloughlin/sha256/blob/master/sha256.c
 * 
 * The Secure Hash Algorithm 256-bit version - https://nvlpubs.nist.gov/nistpubs/FIPS/NIST.FIPS.180-4.pdf
 **/

#include "sha256.h"

// Section 5.1.1 - message input from infile.
int processNextSHA256Block(MessageBlock *M, FILE *infile, uint64_t *numBits, HashStatus *status) {

  int i;
  size_t numBytesRead;

  switch(*status){
    case FINISHED:
      return 0;
    case PAD_ZEROES:
      // We need an all-padding block without the 1 bit.
      for (int i = 0; i < 56; i++)
        M->eight[i] = 0x00;
      M->sixty_four[7] = htobe64(*numBits);
      *status = FINISHED;
      break;
    default:
      // Try to read 64 bytes from the file.
      numBytesRead = fread(M->eight, 1, 64, infile);
      *numBits += (8ULL * ((uint64_t) numBytesRead));
      
      if (numBytesRead < 56) {
        // We can put all padding in this block.
        M->eight[numBytesRead] = 0x80;
        for (i = numBytesRead + 1; i < 56; i++)
          M->eight[i] = 0x00;
        M->sixty_four[7] = htobe64(*numBits);
        *status = FINISHED;
      } else if (numBytesRead < 64) {
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
void nextSHA256Hash(WORD *M, WORD *H) {
  
  WORD W[64];
  WORD a, b, c, d, e, f, g, h, T1, T2;
  int t;

  // for (int i = 0; i < 16; i++) {
  //   W[i] = M[i];
  //   printf("M: %08" PRIx32 "\n", W[i]);
  // }

  for (t = 0; t < 16; t++) {
    W[t] = M[t];
  }

  for (t = 16; t < 64; t++) {
    W[t] = sig1(W[t-2]) + W[t-7] + sig0(W[t-15]) + W[t-16];
  }

  a = H[0]; b = H[1]; c = H[2]; d = H[3];
  e = H[4]; f = H[5]; g = H[6]; h = H[7];

  for (t = 0; t < 64; t++) {
    T1 = h + Sig1(e) + Ch(e, f, g) + K_SHA256[t] + W[t];
    T2 = Sig0(a) + Maj(a, b, c);
    h = g; g = f; f = e; e = d + T1;
    d = c; c = b; b = a; a = T1 + T2;
  }

  H[0] += a; H[1] += b ; H[2] += c; H[3] += d;
  H[4] += e; H[5] += f ; H[6] += g; H[7] += h;
}

void startSHA256(FILE *inFile) {

  // Section 5.3.3
  WORD H[] = {
    0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
    0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19  
  };
  
  // The current padded message block.
  MessageBlock M;
  uint64_t numBits = 0;
  HashStatus status = READ;

  // Read through all of the padded message blocks.
  while (processNextSHA256Block(&M, inFile, &numBits, &status)) {
    // Calculate the next hash value.
    nextSHA256Hash(M.thirty_two, H);
  }

  // Print the hash.
  for (int i = 0; i < 8; i++)
    printf("%08" PRIx32 "", H[i]);
  printf("\n");

  fclose(inFile);
}
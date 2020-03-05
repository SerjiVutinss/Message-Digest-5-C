#ifndef SHA256_H
#define SHA256_H

#include <stdio.h>
#include <inttypes.h>
#include <endian.h>

// Section 2.1
#define WORD uint32_t

// typedef union {
//   uint64_t sixty_four[8];
//   uint32_t thirty_two[16];
//   uint8_t eight[64];
// } Block;

// Section 4.1.2
#define Ch(x, y, z) ((x & y) ^ (~x & z))
#define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHR(x, n) (x >> n)
#define ROTR(x, n) ((x >> n) | (x << (32 - n)))
#define Sig0(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define Sig1(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define sig0(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define sig1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

// // A sixty-four byte block of memory, accessed with different types.
// typedef union {
//   uint64_t sixfour[8];
//   uint32_t threetwo[16];
//   uint8_t eight[64];
// } Block;

// Keep track of where we are in padding the message.
typedef enum {READ, PAD0, FINISH} PADFLAG;

void nextHashSHA256(WORD *M, WORD *H);

#endif
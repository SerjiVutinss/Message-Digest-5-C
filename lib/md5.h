#ifndef MD5_H
#define MD5_H

#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include "custom-types.h"

/**
 * Define a WORD type which is an alias for a 32 bit unsigned integer type.
 **/
// #define WORD uint32_t

// /**
//  * Define a union type, Block, which represents a 512 bit message block to be
//  * processed by the algorithm.
//  **/
// typedef union Block {
//   uint64_t sixty_four[8];
//   uint32_t thirty_two[16];
//   uint8_t eight[64];
// } Block;

/** Define Constants for MD5 Algorithm **/

/** Define K, the binary integer part of the sines or integers (Radians)
 * as constants.
 * i.e. for i = 0 to 63: K[i] := floor(232 × abs (sin(i + 1)))
 **/
static const WORD K_MD5[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,

    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,

    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,

    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};

/**
 * RFC 1321 3.3 - Step 3. Initialise MD Buffer aka Magic Initiliasation Constants.
 * 
 * A four word buffer is used to compute the message digest, i.e. 4 x 32 bit registers.
 **/
static WORD H_MD5[] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476};

/**
 * Define S, the per round bit-shift amounts.
 **/
static const uint32_t S[64] = {
    7, 12, 17, 22,
    7, 12, 17, 22,
    7, 12, 17, 22,
    7, 12, 17, 22,

    5, 9, 14, 20,
    5, 9, 14, 20,
    5, 9, 14, 20,
    5, 9, 14, 20,

    4, 11, 16, 23,
    4, 11, 16, 23,
    4, 11, 16, 23,
    4, 11, 16, 23,

    6, 10, 15, 21,
    6, 10, 15, 21,
    6, 10, 15, 21,
    6, 10, 15, 21};

// End Constants

/**
 * RFC 1321 3.4 - Step 4. Process Message in 16-Word Blocks
 * 
 * Define four auxiliary functions that each take as input three 32-bit words and 
 * produce as output one 32-bit word.
 **/

// #define F(x,  y,  z) ((x & y) | ((~x) & z))
static uint32_t F(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & y) | ((~x) & z));
}

// #define G( x,  y,  z) ((x & z) | (y & (~z)))
static uint32_t G(uint32_t x, uint32_t y, uint32_t z)
{
    return ((x & z) | (y & (~z)));
}

// #define H( x,  y,  z) (x ^ y ^ z)
static uint32_t H(uint32_t x, uint32_t y, uint32_t z)
{
    return (x ^ y ^ z);
}

// #define I( x,  y,  z) (y ^ (x | (~z)))
static uint32_t I(uint32_t x, uint32_t y, uint32_t z)
{
    return (y ^ (x | (~z)));
}

/**
 *  Also define the Rotate Left (ROTL) function - rotate X left by C number of bits.
 * 
 *  This function will be used by the helper methods below.
 **/
// #define ROTL(x, c) (((x) << (c)) | ((x) >> (32 - (c))))
static uint32_t ROTL(uint32_t x, uint32_t c)
{
    return (((x) << (c)) | ((x) >> (32 - (c))));
}

/**
 * * RFC 1321 3.4 - Step 4. Define helper functions for rounds (outlined in Appendix 3.A)
 * 
 * FF will be performed in Round 1.
 * GG will be performed in Round 2.
 * HH will be performed in Round 3.
 * II will be performed in Round 4.
 **/

// Round 1
static void FF(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t k, uint32_t s, uint32_t ac)
{
    uint32_t sum = (*a + F(b, c, d) + k + ac);
    uint32_t f = F(b, c, d);
    *a = b + ROTL(sum, s);
}
// Round 2
static void GG(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
    uint32_t sum = (*a + G(b, c, d) + x + ac);
    uint32_t g = G(b, c, d);
    *a = b + ROTL(sum, s);
}
// Round 3
static void HH(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
    uint32_t sum = (*a + H(b, c, d) + x + ac);
    uint32_t h = H(b, c, d);
    *a = b + ROTL(sum, s);
}
// Round 4
static void II(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac)
{
    uint32_t sum = (*a + I(b, c, d) + x + ac);
    uint32_t i = I(b, c, d);
    *a = b + ROTL(sum, s);
}

/**
 * Prototypes for unimplemented methods
 **/
// int processNextBlock(MessageBlock *M, FILE *infile, size_t *numBits, enum HashStatus *status);
static int processFileBlock(MessageBlock *M, FILE *infile, size_t *numBits, enum HashStatus *status);
static int processStringBlock(MessageBlock *M, char *buffer, size_t *numBits, enum HashStatus *status);
static int processNextBlock(MessageBlock *M, size_t numBytesRead, size_t *numBits, enum HashStatus *status);
static void nextHash(MessageBlock *M, WORD *H);

WORD *startMD5HashFile(FILE *inFile);
WORD *startMD5StringHash(char *inputString);

WORD *startMD5HashData(HashOptions hashData);

// void startMD5StringHash(char *string);

#endif
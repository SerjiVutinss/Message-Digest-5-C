#ifndef MD5_ALG_H
#define MD5_ALG_H

#include "message-block.h"

#include <stdio.h>
#include <inttypes.h>

#define ROTL(x, n) (x << n) | (x >> (32 - n))

#define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
#define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | (~z)))


// #define F(x, y, z) (x & y) | (~x & z)
// #define G(x, y, z) (x & z) | (y & ~z)
// #define H(x, y, z) (x ^ y ^ z)
// #define I(x, y, z) (y ^ (x | ~z))

void nextHashMD5(WORD *M, WORD *H);

#endif
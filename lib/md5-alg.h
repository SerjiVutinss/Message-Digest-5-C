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

// #define FF(a,b,c,d,x,s,t) (a = b + ROTL((a + F(b,c,d) + x + t), s))
// #define GG(a,b,c,d,x,s,t) (a = b + ROTL((a + G(b,c,d) + x + t), s))
// #define HH(a,b,c,d,x,s,t) (a = b + ROTL((a + H(b,c,d) + x + t), s))
// #define II(a,b,c,d,x,s,t) (a = b + ROTL((a + I(b,c,d) + x + t), s))
// #define F(x, y, z) (x & y) | (~x & z)
// #define G(x, y, z) (x & z) | (y & ~z)
// #define H(x, y, z) (x ^ y ^ z)
// #define I(x, y, z) (y ^ (x | ~z))

void nextHashMD5(Block *M, WORD *H);

#endif
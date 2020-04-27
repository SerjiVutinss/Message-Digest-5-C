#ifndef SHA256_H
#define SHA256_H

#include <stdio.h>
#include <inttypes.h>
#include <endian.h>

#include "lib/message-block.h"

// Section 4.1.2
#define Ch(x, y, z) ((x & y) ^ (~x & z))
#define Maj(x, y, z) ((x & y) ^ (x & z) ^ (y & z))
#define SHR(x, n) (x >> n)
#define ROTR(x, n) ((x >> n) | (x << (32 - n)))
#define Sig0(x) (ROTR(x,  2) ^ ROTR(x, 13) ^ ROTR(x, 22))
#define Sig1(x) (ROTR(x,  6) ^ ROTR(x, 11) ^ ROTR(x, 25))
#define sig0(x) (ROTR(x,  7) ^ ROTR(x, 18) ^ SHR(x, 3))
#define sig1(x) (ROTR(x, 17) ^ ROTR(x, 19) ^ SHR(x, 10))

void nextHashSHA256(WORD *M, WORD *H);

#endif
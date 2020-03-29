#ifndef MD5_ALG_H
#define MD5_ALG_H

#include "message-block.h"

#include <stdio.h>
#include <inttypes.h>

#define ROTL(x, n) ((x) << (n) | (x) >> (32 - (n)))
#define F(x, y, z) ((z) ^ ((x) & ((y) ^ (z))))
#define G(x, y, z) ((y) ^ ((z) & ((x) ^ (y))))
#define H(x, y, z) ((x) ^ (y) ^ (z))
#define I(x, y, z) ((y) ^ ((x) | ~(z)))

void nextHashMD5(WORD *M, WORD *H);

// F(X,Y,Z) = XY v not(X) Z
// G(X,Y,Z) = XZ v Y not(Z)
// H(X,Y,Z) = X xor Y xor Z
// I(X,Y,Z) = Y xor (X v not(Z))

#endif
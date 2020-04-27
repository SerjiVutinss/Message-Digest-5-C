#include "md5-alg.h"
#include "message-block.h"
#include <inttypes.h>
#include <byteswap.h>

// k = word
void FF(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t k, uint32_t s, uint32_t ac) {
    uint32_t sum = (*a + F(b, c, d) + k + ac);
    uint32_t f = F(b, c, d);
    // printf("rotateLeft(%x + %x + %x + %x, %d)\n", *a, f, ac, k, s);
    *a = b + ROTL(sum, s);
}

void GG(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
    uint32_t sum = (*a + G(b, c, d) + x + ac);//(a) += F ((b), (c), (d)) + (x) + (UINT4)(ac);
    uint32_t g = G(b, c, d);
    //printf("rotateLeft(%x + %x + %x + %x, %d)\n", *a, g, ac, x, s);
    *a = b + ROTL(sum, s);//(a) = ROTATE_LEFT ((a), (s));
}

void HH(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
    uint32_t sum = (*a + H(b, c, d) + x + ac);
    uint32_t h = H(b, c, d);
    // printf("rotateLeft(%x + %x + %x + %x, %d)\n", *a, h, ac, x, s);
    *a = b + ROTL(sum, s);
}

void II(uint32_t *a, uint32_t b, uint32_t c, uint32_t d, uint32_t x, uint32_t s, uint32_t ac) {
    uint32_t sum = (*a + I(b, c, d) + x + ac);
    uint32_t i = I(b, c, d);
    // printf("rotateLeft(%x + %x + %x + %x, %d)\n", *a, i, ac, x, s);
    *a = b + ROTL(sum, s);
}

const uint32_t S[64] = {
    7, 12, 17, 22, 
    7, 12, 17, 22, 
    7, 12, 17, 22, 
    7, 12, 17, 22,

    5,  9, 14, 20, 
    5,  9, 14, 20, 
    5,  9, 14, 20, 
    5,  9, 14, 20,
    
    4, 11, 16, 23, 
    4, 11, 16, 23, 
    4, 11, 16, 23, 
    4, 11, 16, 23,
    
    6, 10, 15, 21, 
    6, 10, 15, 21, 
    6, 10, 15, 21, 
    6, 10, 15, 21
};

const WORD K_MD5[64] = { 
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
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

uint32_t leftRotate(uint32_t x, int n) {
    return (x << n) | (x >> (32 - n));
}

uint32_t Reverse32(uint32_t value) 
{
    return (((value & 0x000000FF) >> 24) |
            ((value & 0x0000FF00) >>  8) |
            ((value & 0x00FF0000) <<  8) |
            ((value & 0xFF000000) >> 24));
}

// // For each block...
// void nextHashMD5(WORD *M, WORD *H) {

//     uint32_t A, B, C, D, i, F, G, temp_d;

//     A = H[0];
//     B = H[1];
//     C = H[2];
//     D = H[3];

//     // 16 x 32-bit WORDs
//     WORD W[16];

//     // Copy M into X
//     for (i = 0; i < 16; i++) {
//         W[i] = M[i];
//         W[i] = __bswap_32(W[i]);
//         printf("M: %08" PRIx32 "\n", W[i]);
//     }

//     // for (int i = 0; i < 16; i++) {
//     //     printf("X: %08" PRIx32 "\n", X[i]);
//     // }

//     for (i = 0; i < 64; i++){

//         if (i < 16){
//             F = (B & C) | (~B & D);
//             G = i;

//         } else if (i < 32){
//             F = (D & B) | (~D & C);
//             G = ((5 * i) + 1) % 16;

//         } else if (i < 48){
//             F = B ^ C ^ D;
//             G = ((3 * i) + 5) % 16;

//         } else {
//             F = C ^ (B | ~D);
//             G = (7 * i) % 16;
//         }

//         temp_d = D;
//         D = C;
//         C = B;
//         B += leftRotate((A + F + K_MD5[i] + W[G]), S[i]);
//         A = temp_d;
//     }

//     H[0] += A;
//     H[1] += B;
//     H[2] += C;
//     H[3] += D;

//     printf("\n");
//     printf("A: %08" PRIx32 "\n", A);
//     printf("B: %08" PRIx32 "\n", B);
//     printf("C: %08" PRIx32 "\n", C);
//     printf("D: %08" PRIx32 "\n", D);

//     printf("\n");
// }


void nextHashMD5(Block *M, WORD *H) {

    // for (int i = 0; i < 16; i++) {
    //     // W[i] = M[i];
    //     printf("M: %08" PRIx32 "\n", M->thirty_two[i]);
    // }

    uint32_t copyH[4];
    copyH[0] = Reverse32(H[0]);
    copyH[1] = Reverse32(H[1]);
    copyH[2] = Reverse32(H[2]);
    copyH[3] = Reverse32(H[3]);

    /* Round 1 */
    FF(&H[0], H[1], H[2], H[3], M->thirty_two[0], S[0], K_MD5[0]); /* 1 */
    FF(&H[3], H[0], H[1], H[2], M->thirty_two[1], S[1], K_MD5[1]); /* 2 */
    FF(&H[2], H[3], H[0], H[1], M->thirty_two[2], S[2], K_MD5[2]); /* 3 */
    FF(&H[1], H[2], H[3], H[0], M->thirty_two[3], S[3], K_MD5[3]); /* 4 */
    FF(&H[0], H[1], H[2], H[3], M->thirty_two[4], S[4], K_MD5[4]); /* 5 */
    FF(&H[3], H[0], H[1], H[2], M->thirty_two[5], S[5], K_MD5[5]); /* 6 */
    FF(&H[2], H[3], H[0], H[1], M->thirty_two[6], S[6], K_MD5[6]); /* 7 */
    FF(&H[1], H[2], H[3], H[0], M->thirty_two[7], S[7], K_MD5[7]); /* 8 */
    FF(&H[0], H[1], H[2], H[3], M->thirty_two[8], S[8], K_MD5[8]); /* 9 */
    FF(&H[3], H[0], H[1], H[2], M->thirty_two[9], S[9], K_MD5[9]); /* 10 */
    FF(&H[2], H[3], H[0], H[1], M->thirty_two[10], S[10], K_MD5[10]); /* 11 */
    FF(&H[1], H[2], H[3], H[0], M->thirty_two[11], S[11], K_MD5[11]); /* 12 */
    FF(&H[0], H[1], H[2], H[3], M->thirty_two[12], S[12], K_MD5[12]); /* 13 */
    FF(&H[3], H[0], H[1], H[2], M->thirty_two[13], S[13], K_MD5[13]); /* 14 */
    FF(&H[2], H[3], H[0], H[1], M->thirty_two[14], S[14], K_MD5[14]); /* 15 */
    FF(&H[1], H[2], H[3], H[0], M->thirty_two[15], S[15], K_MD5[15]); /* 16 */
    // printf("\n");
//    /* Round 2 */
    GG(&H[0], H[1], H[2], H[3], M->thirty_two[1], S[16], K_MD5[16]); /* 17 */
    GG(&H[3], H[0], H[1], H[2], M->thirty_two[6], S[17], K_MD5[17]); /* 18 */
    GG(&H[2], H[3], H[0], H[1], M->thirty_two[11], S[18], K_MD5[18]); /* 19 */
    GG(&H[1], H[2], H[3], H[0], M->thirty_two[0], S[19], K_MD5[19]); /* 20 */
    GG(&H[0], H[1], H[2], H[3], M->thirty_two[5], S[20], K_MD5[20]); /* 21 */
    GG(&H[3], H[0], H[1], H[2], M->thirty_two[10], S[21], K_MD5[21]); /* 22 */
    GG(&H[2], H[3], H[0], H[1], M->thirty_two[15], S[22], K_MD5[22]); /* 23 */
    GG(&H[1], H[2], H[3], H[0], M->thirty_two[4], S[23], K_MD5[23]); /* 24 */
    GG(&H[0], H[1], H[2], H[3], M->thirty_two[9], S[24], K_MD5[24]); /* 25 */
    GG(&H[3], H[0], H[1], H[2], M->thirty_two[14], S[25], K_MD5[25]); /* 26 */
    GG(&H[2], H[3], H[0], H[1], M->thirty_two[3], S[26], K_MD5[26]); /* 27 */
    GG(&H[1], H[2], H[3], H[0], M->thirty_two[8], S[27], K_MD5[27]); /* 28 */
    GG(&H[0], H[1], H[2], H[3], M->thirty_two[13], S[28], K_MD5[28]); /* 29 */
    GG(&H[3], H[0], H[1], H[2], M->thirty_two[2], S[29], K_MD5[29]); /* 30 */
    GG(&H[2], H[3], H[0], H[1], M->thirty_two[7], S[30], K_MD5[30]); /* 31 */
    GG(&H[1], H[2], H[3], H[0], M->thirty_two[12], S[31], K_MD5[31]); /* 32 */
    // printf("\n");
//    /* Round 3 */
    HH(&H[0], H[1], H[2], H[3], M->thirty_two[5], S[32], K_MD5[32]); /* 33 */
    HH(&H[3], H[0], H[1], H[2], M->thirty_two[8], S[33], K_MD5[33]); /* 34 */
    HH(&H[2], H[3], H[0], H[1], M->thirty_two[11], S[34], K_MD5[34]); /* 35 */
    HH(&H[1], H[2], H[3], H[0], M->thirty_two[14], S[35], K_MD5[35]); /* 36 */
    HH(&H[0], H[1], H[2], H[3], M->thirty_two[1], S[36], K_MD5[36]); /* 37 */
    HH(&H[3], H[0], H[1], H[2], M->thirty_two[4], S[37], K_MD5[37]); /* 38 */
    HH(&H[2], H[3], H[0], H[1], M->thirty_two[7], S[38], K_MD5[38]); /* 39 */
    HH(&H[1], H[2], H[3], H[0], M->thirty_two[10], S[39], K_MD5[39]); /* 40 */
    HH(&H[0], H[1], H[2], H[3], M->thirty_two[13], S[40], K_MD5[40]); /* 41 */
    HH(&H[3], H[0], H[1], H[2], M->thirty_two[0], S[41], K_MD5[41]); /* 42 */
    HH(&H[2], H[3], H[0], H[1], M->thirty_two[3], S[42], K_MD5[42]); /* 43 */
    HH(&H[1], H[2], H[3], H[0], M->thirty_two[6], S[43], K_MD5[43]); /* 44 */
    HH(&H[0], H[1], H[2], H[3], M->thirty_two[9], S[44], K_MD5[44]); /* 45 */
    HH(&H[3], H[0], H[1], H[2], M->thirty_two[12], S[45], K_MD5[45]); /* 46 */
    HH(&H[2], H[3], H[0], H[1], M->thirty_two[15], S[46], K_MD5[46]); /* 47 */
    HH(&H[1], H[2], H[3], H[0], M->thirty_two[2], S[47], K_MD5[47]); /* 48 */
    // printf("\n");
//    /* Round 4 */
    II(&H[0], H[1], H[2], H[3], M->thirty_two[0], S[48], K_MD5[48]); /* 49 */
    II(&H[3], H[0], H[1], H[2], M->thirty_two[7], S[49], K_MD5[49]); /* 50 */
    II(&H[2], H[3], H[0], H[1], M->thirty_two[14], S[50], K_MD5[50]); /* 51 */
    II(&H[1], H[2], H[3], H[0], M->thirty_two[5], S[51], K_MD5[51]); /* 52 */
    II(&H[0], H[1], H[2], H[3], M->thirty_two[12], S[52], K_MD5[52]); /* 53 */
    II(&H[3], H[0], H[1], H[2], M->thirty_two[3], S[53], K_MD5[53]); /* 54 */
    II(&H[2], H[3], H[0], H[1], M->thirty_two[10], S[54], K_MD5[54]); /* 55 */
    II(&H[1], H[2], H[3], H[0], M->thirty_two[1], S[55], K_MD5[55]); /* 56 */
    II(&H[0], H[1], H[2], H[3], M->thirty_two[8], S[56], K_MD5[56]); /* 57 */
    II(&H[3], H[0], H[1], H[2], M->thirty_two[15], S[57], K_MD5[57]); /* 58 */
    II(&H[2], H[3], H[0], H[1], M->thirty_two[6], S[58], K_MD5[58]); /* 59 */
    II(&H[1], H[2], H[3], H[0], M->thirty_two[13], S[59], K_MD5[59]); /* 60 */
    II(&H[0], H[1], H[2], H[3], M->thirty_two[4], S[60], K_MD5[60]); /* 61 */
    II(&H[3], H[0], H[1], H[2], M->thirty_two[11], S[61], K_MD5[61]); /* 62 */
    II(&H[2], H[3], H[0], H[1], M->thirty_two[2], S[62], K_MD5[62]); /* 63 */
    II(&H[1], H[2], H[3], H[0], M->thirty_two[9], S[63], K_MD5[63]); /* 64 */
    // printf("\n");

    H[0] += copyH[0];
    H[1] += copyH[1];
    H[2] += copyH[2];
    H[3] += copyH[3];
}
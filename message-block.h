#ifndef MESSAGE_BLOCK_H
#define MESSAGE_BLOCK_H

#include <inttypes.h>

typedef union {
  uint64_t sixty_four[8];
  uint32_t thirty_two[16];
  uint8_t eight[64];
} Block;


void createFullyPaddedBlock(Block *block, uint64_t len);

void convertBlockToHostEndianness(Block *block);

#endif
#include "message-block.h"

#include <endian.h>
#include "message-info.h"

void convertBlockToHostEndianness(Block *block){
    for (int i = 0; i < 16; i++) {
        block->thirty_two[i] = be32toh(block->thirty_two[i]);
    }
}

void createFullyPaddedBlock(Block *block, uint64_t len) {
    for(int i = 0; i < 56; i++) {
        block->eight[i] = 0x00;
    }
    block->sixty_four[7] = htobe64(len);
}
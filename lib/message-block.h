#ifndef MESSAGE_BLOCK_H
#define MESSAGE_BLOCK_H

#include <inttypes.h>
#include <endian.h>

#include "custom-types.h"

void createFullyPaddedBlock(MessageBlock *block, uint64_t len);

void convertBlockToHostEndianness(MessageBlock *block);

#endif
/*
Rough outline:
- Begin at the end of the kernel
- Have a bitmap for free and used pages
- Allocate one after the other
*/

#include <stdint.h>
#include "pmm.h"
#include "../lib/string.h"

uint8_t* bitmap = (uint8_t*)(&end_kernel);
uint8_t* blockStart;
uint32_t nBlocks;

void PMM_Init() {
    uint32_t availMem = 1024 * 1024 * 1024; // 1 GiB
    nBlocks = availMem / BLOCK_SIZE;
    uint32_t nBuckets = nBlocks / BUCKET_SIZE;

    if(nBuckets * BUCKET_SIZE < nBlocks) {
        nBuckets++;
    }

    memset(bitmap, 0, nBlocks); // Clear the bitmap
    blockStart = (uint8_t*)BLOCK_ALIGN((uint32_t)(bitmap + nBlocks));
}

static uint32_t PMM_FirstFreeBlock() {
    // Start loop at blockStart
    for(int i = 0; i < nBlocks; i++) {
        if(!ISSET(i)) {
            return i;
        }
    }
    return (uint32_t)-1;
}

uint32_t PMM_AllocBlock() {
    uint32_t freeBlock = PMM_FirstFreeBlock();
    SETBIT(freeBlock);
    return freeBlock;
}

void PMM_FreeBlock(uint32_t blockNumber) {
    CLEARBIT(blockNumber);
}
/*  Author: Joseph Shimel
    Date:   7/28/21 */

#include <stdint.h>
#include "pmm.h"
#include "../lib/string.h"
#include "../lib/stdio.h"
#include "../lib/system.h"

uint8_t* bitmap = (uint8_t*)(&end_kernel);
uint8_t* blockStart;
uint32_t nBlocks;
uint32_t bitmapSize;

/*  Initializes the physical memory manager's bitmap */
void PMM_Init() {
    uint32_t availMem = K * M; // 1 GiB
    nBlocks = availMem / BLOCK_SIZE;
    bitmapSize = nBlocks / BUCKET_SIZE;

    if(bitmapSize * BUCKET_SIZE < nBlocks) {
        bitmapSize++;
    }

    memset(bitmap, 0, bitmapSize); // Clear the bitmap
    blockStart = (uint8_t*)BLOCK_ALIGN((uint32_t)(bitmap + bitmapSize));
}

/*  Finds the first unused block

    @return first unused block */
static uint32_t PMM_FirstFreeBlock() {
    // Start loop at blockStart
    for(int i = 0; i < nBlocks; i++) {
        if(!ISSET(i)) {
            return i;
        }
    }
    return (uint32_t)-1;
}

/*  Finds the next first free block, sets it as used, and returns the 
    corresponding block number.

    @return The next block number of the next free block */
uint32_t PMM_AllocBlock() {
    uint32_t freeBlock = PMM_FirstFreeBlock();
    SETBIT(freeBlock);
    return freeBlock;
}

/*  Sets the bit of a block to make it unused 

    @param blockNumber  The number to set to free */
void PMM_FreeBlock(uint32_t blockNumber) {
    CLEARBIT(blockNumber);
}
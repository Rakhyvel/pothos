#ifndef PMM_H
#define PMM_H

#define BLOCK_SIZE 4096     // bytes per block
#define BUCKET_SIZE 8       // blocks per bucket (32 KiB)

#define SETBIT(i) bitmap[i / BUCKET_SIZE] = bitmap[i / BUCKET_SIZE] | (1 << (i % BUCKET_SIZE))
#define CLEARBIT(i) bitmap[i / BUCKET_SIZE] = bitmap[i / BUCKET_SIZE] & (~(1 << (i % BUCKET_SIZE)))
#define ISSET(i) ((bitmap[i / BUCKET_SIZE] >> (i % BUCKET_SIZE)) & 0x1)
#define GET_BUCKET32(i) (*((uint32_t*) &bitmap[i / 32]))

#define BLOCK_ALIGN(addr) (((addr) & 0xFFFFF000) + 0x1000) // Returns an aligned address

extern uint32_t end_kernel; // Defined in linker.ld, end of the kernel

void PMM_Init();

#endif
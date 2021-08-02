#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGE_SIZE 4096

// Alignment related macro
#define IS_ALIGN(addr) ((((uint32_t)(addr)) | 0xFFFFF000) == 0)
#define PAGE_ALIGN(addr) ((((uint32_t)(addr)) & 0xFFFFF000) + 0x1000)

// Paging register manipulation macro
#define SET_PGBIT(cr0) (cr0 = cr0 | 0x80000000)
#define CLEAR_PSEBIT(cr4) (cr4 = cr4 & 0xffffffef)

// Defone some address calculation macro
#define PAGEDIR_INDEX(vaddr) (((uint32_t)vaddr) >> 22)
#define PAGETBL_INDEX(vaddr) ((((uint32_t)vaddr) >>12) & 0x3ff)
#define PAGEFRAME_INDEX(vaddr) (((uint32_t)vaddr) & 0xfff)

// page_directory_t
typedef struct page_dir_entry {
    unsigned int present    : 1;
    unsigned int rw         : 1;
    unsigned int user       : 1;
    unsigned int w_through  : 1;
    unsigned int cache      : 1;
    unsigned int access     : 1;
    unsigned int reserved   : 1;
    unsigned int page_size  : 1;
    unsigned int global     : 1;
    unsigned int available  : 3;
    unsigned int frame      : 20;
} page_dir_entry_t;

typedef struct page_table_entry {
    unsigned int present    : 1;
    unsigned int rw         : 1;
    unsigned int user       : 1;
    unsigned int reserved   : 2;
    unsigned int accessed   : 1;
    unsigned int dirty      : 1;
    unsigned int reserved2  : 2;
    unsigned int available  : 3;
    unsigned int frame      : 20;
} page_table_entry_t;

typedef struct page_table {
    page_table_entry_t pages[1024];
} page_table_t;

typedef struct page_directory {
    // The actual page directory entries(note that the frame number it stores is physical address)
    page_dir_entry_t tables[1024];
    // We need a table that contains virtual address, so that we can actually get to the tables
    page_table_t * ref_tables[1024];
} page_directory_t;

// Defined in pmm.c
extern uint8_t * bitmap;
extern uint32_t  bitmapSize;

// Defined in boot.asm
extern page_directory_t* boot_page_directory;

void Paging_Init();
void Paging_AllocatePage(page_directory_t * dir, uint32_t virtual_addr, uint32_t frame, int is_kernel, int is_writable);
void Paging_AllocateRegion(struct page_directory* dir, uint32_t start_va, uint32_t end_va, int iden_map, int is_kernel, int is_writable);
void Paging_SetPageDirectory(struct page_directory* page_dir, uint32_t phys);
void Paging_Enable();

#endif
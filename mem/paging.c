#include "paging.h"

static void dumb_kmalloc();
static void* virtual2phys(page_directory_t* dir, void* virtual_addr);

void Paging_Init() {
    /*
    Before all this is done I need to initialize the Physical Memory Manager

    1. Allocate a small bit of space for a new page directory
    2. Allocate pages for the first MiB to where it needs to be for kernel data and code
    3. Allocate pages for kernel heap (after kernel code and data)
    4. Register a new page handler interrupt (do later)
    5. Set the cr3 register to point to new page directory
    6. Enable paging with cr0 and cr4 register
    7. 
    */
}

/*
    Sets the cr0 and cr4 registers to enable paging */
void Paging_Enable() {
    uint32_t cr0, cr4;

    asm volatile("mov %%cr4, %0" : "=r"(cr4));
    CLEAR_PSEBIT(cr4);
    asm volatile("mov %0, %%cr4" :: "r"(cr4));

    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    SET_PGBIT(cr0);
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}
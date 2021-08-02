/*  Author: Joseph Shimel
    Date:   7/27/21 */
    
#include "paging.h"
#include "./kheap.h"
#include "./pmm.h"
#include "../lib/string.h"
#include "../lib/system.h"

uint8_t* tempMem;
int isPagingEnabled = 0;
int isKernelHeapEnabled = 0;

page_directory_t *kpage_dir;

static void* dumb_kmalloc(uint32_t size, int align);
static void* virtual2phys(page_directory_t* dir, void* virtual_addr);

/*  Initializes kernel page directory.

    v 0xC0000000
    |--Kernel Code: 4M--|--Kernel Heap: 48M--| */
void Paging_Init() {
    tempMem = bitmap + bitmapSize;

    kpage_dir = dumb_kmalloc(sizeof(page_directory_t), 1);
    memset(kpage_dir, 0, sizeof(page_directory_t));


    // Alloc 4 MiB of pages for kernel
    for(int i = LOAD_MEMORY_ADDRESS; i < LOAD_MEMORY_ADDRESS + 4 * M; i += PAGE_SIZE) {
        Paging_AllocatePage(kpage_dir, i, 0, 1, 1);
    }


    // Alloc space after kernel for kernel heap
    for(int i = LOAD_MEMORY_ADDRESS + 4 * M; i < LOAD_MEMORY_ADDRESS + 4 * M + KHEAP_INITIAL_SIZE; i += PAGE_SIZE) {
        Paging_AllocatePage(kpage_dir, i, 0, 1, 1);
    }


    Paging_SetPageDirectory(kpage_dir, 0);

    Paging_Enable();

    Paging_AllocateRegion(kpage_dir, 0x0, 0x10000, 1, 1, 1);
}

/*  Allocates a region in a page directory

    @param dir          A pointer to the page directory
    @param start_va     Starting virtual address of region
    @param end_va       Ending virtual address of region
    @param iden_map     Whether to identity map the region or not
    @param is_kernel    Unused
    @param is_writable  Unused */
void Paging_AllocateRegion(struct page_directory* dir, uint32_t start_va, uint32_t end_va, int iden_map, int is_kernel, int is_writable) {
    uint32_t start = start_va & 0xFFFFF000;
    uint32_t end = end_va & 0xFFFFF000;
    while(start <= end) {
        if(iden_map) {
            Paging_AllocatePage(dir, start, start / PAGE_SIZE, is_kernel, is_writable);
        } else {
            Paging_AllocatePage(dir, start, 0, is_kernel, is_writable);
        }
        start += PAGE_SIZE;
    }
}

/*  Switches the page directory by setting the register cr3
    
    @param page_dir A pointer to a page directory
    @param phys     Whether or not the address to the page directory is a 
                    physical or virtual address */
void Paging_SetPageDirectory(struct page_directory* page_dir, uint32_t phys) {
    uint32_t t;
    if(!phys)
        t = (uint32_t)virtual2phys(boot_page_directory, page_dir);
    else
        t = (uint32_t)page_dir;

    asm volatile("mov %0, %%cr3" :: "r"(t));
}

/*  Allocates memory in the kernel's temporary heap.

    @param size     Size of the region to allocate
    @param align    Whether or not to align the region on a 4KiB page boundary

    @return         A virtual address of the region of memory allocated */
void* dumb_kmalloc(uint32_t size, int align) {
    void* retval = tempMem;
    if(align && !IS_ALIGN(retval)) {
        retval = (void*)PAGE_ALIGN(retval);
    }
    tempMem += size;
    return retval;
}

/*  If virtual address is not mapped in page directory structure, allocate 
    tables and/or pages for directory

    @param dir          A pointer to the page directory used
    @param virtual_addr Virtual address to check for in directory
    @param frame        Used to predefine pageframe if it doesnt exist. Set to
                        0 to use PMM instead
    @param is_kernel    Unused
    @param is_writable  Unused */
void Paging_AllocatePage(page_directory_t * dir, uint32_t virtual_addr, uint32_t frame, int is_kernel, int is_writable) {
    page_table_t* table;
    if(!dir) {
        for(;;);
    }
    uint32_t pageDirIndex = PAGEDIR_INDEX(virtual_addr);
    uint32_t pageTableIndex = PAGETBL_INDEX(virtual_addr);

    table = dir->ref_tables[pageDirIndex];
    // If table doesnt exist, alloc table and put into directory
    if(!table) {
        if(!isKernelHeapEnabled) {
            table = dumb_kmalloc(sizeof(page_table_t), 1);
        } else {
            // TODO: When you enable kernel heap
            // table = kmalloc_a(sizeof(page_table_t));
        }

        memset(table, 0, sizeof(page_table_t));

        uint32_t addr = (uint32_t)virtual2phys(kpage_dir, table);
        dir->tables[pageDirIndex].frame = addr >> 12;
        dir->tables[pageDirIndex].present = 1;
        dir->tables[pageDirIndex].rw = 1;
        dir->tables[pageDirIndex].user = 1;
        dir->tables[pageDirIndex].page_size = 0;
        dir->ref_tables[pageDirIndex] = table;
    }

    // If page doesnt exist, alloc page
    if(!table->pages[pageTableIndex].present) {
        uint32_t t;
        if (frame) {
            t = frame;
        } else {
            t = PMM_AllocBlock();
        }
        table->pages[pageTableIndex].frame = t;
        table->pages[pageTableIndex].present = 1;
        table->pages[pageTableIndex].rw = 1;
        table->pages[pageTableIndex].user = 1;
    }
}

/*  Sets the cr0 and cr4 registers to enable paging */
void Paging_Enable() {
    uint32_t cr0, cr4;

    asm volatile("mov %%cr4, %0" : "=r"(cr4));
    CLEAR_PSEBIT(cr4);
    asm volatile("mov %0, %%cr4" :: "r"(cr4));

    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    SET_PGBIT(cr0);
    asm volatile("mov %0, %%cr0" :: "r"(cr0));
}


/*  Converts a virtual address to a physical address
    
    @param dir          A pointer to the page directory structure
    @param virtualAddr  The virtual address to convert
    @return             A physical address based on the page directory given */
static void* virtual2phys(page_directory_t* dir, void* virtualAddr) {
    // If paging isn't enabled, physical address is just virtual minus 0xc0000000
    if(!isPagingEnabled) {
        return (void*)(virtualAddr - LOAD_MEMORY_ADDRESS);
    }

    // Find indices for directory, table, and frame
    uint32_t pageDirIndex = PAGEDIR_INDEX(virtualAddr);
    uint32_t pageTableIndex = PAGETBL_INDEX(virtualAddr);
    uint32_t pageFrameOffset = PAGEFRAME_INDEX(virtualAddr);

    // Check to see that table reference entry in directory is valid
    if(!dir->ref_tables[pageDirIndex]) {
        for(;;);
    }

    // Check to see that frame reference entry in table is present
    page_table_t* table = dir->ref_tables[pageDirIndex];
    if(!table->pages[pageTableIndex].present) {
        printf("Whoa!\n");
        for(;;);
    }

    // Calculate frame, append frame offset
    uint32_t physicalAddress = table->pages[pageTableIndex].frame;
    physicalAddress = (physicalAddress << 12) + pageFrameOffset;
    return physicalAddress;
}
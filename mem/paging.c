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
/*
The GDT has 64 entries that tell the processor about where segments start/stop and what flags those segments have.
We need a GDT structure, and a GDT descriptor. The descriptor has a 16-bit value for the length, and a 32-bit value
for the address of the GDT structure.
*/

#ifndef _BOOT_GDT
#define _BOOT_GDT

#include <stdint.h>

// Number of global descriptors
#define NUM_DESCRIPTORS 8

// Gdt related structures
typedef struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_middle;
    uint8_t  access;
    uint8_t  granularity;
    uint8_t  base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct gdt_ptr
{
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

// Extern asm functions
extern void GDT_Flush(uint32_t gdt_ptr);

// gdt functions
void GDT_Init();
void GDT_SetEntry(int index, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);

extern gdt_entry_t gdt_entries[NUM_DESCRIPTORS];

#endif
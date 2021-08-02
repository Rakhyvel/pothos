/*	kernel.c

	The kernel's main function is called by boot.asm. This starts the OS

	Author: Joseph Shimel
	Date: 7/25/21
*/

#include "./gdt.h"
#include "./interrupt.h"
#include "../devices/vga.h"
#include "../devices/timer.h"
#include "../devices/keyboard.h"
#include "../lib/stdio.h"
#include "../lib/string.h"
#include "../boot/multiboot.h"
#include "../mem/pmm.h"
#include "../mem/paging.h"
#include "../devices/serial.h"

int main(multiboot_info_t* info) 
{
	VGA_Init();
	GDT_Init();
	IDT_Init();

	PMM_Init();
	Paging_Init();

	printf("PothOS 0.0\nRAM: %d bytes\n>", 1096 * M);

	for(;;);
	return 0;
}
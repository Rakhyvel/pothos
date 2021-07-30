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

int main(multiboot_info_t* info) 
{
	VGA_Init();
	GDT_Init();
	IDT_Init();

	PMM_Init();

	Timer_Init(1);
	Keyboard_Init();

	printf("PothOS 0.0\nKernel ends at: 0x");
	char lol[255];
	itoa(lol, &end_kernel, 16);
	printf(lol);
	printf("\n> ");

	PMM_AllocBlock();

	for(;;);
	return 0;
}
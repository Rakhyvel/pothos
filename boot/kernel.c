/*	kernel.c

	The kernel's main function is called by boot.asm. This starts the OS

	Author: Joseph Shimel
	Date: 7/25/21
*/

#include "./gdt.h"
#include "../threads/interrupt.h"
#include "../devices/vga.h"
#include "../devices/timer.h"
#include "../lib/stdio.h"

int main() 
{
	VGA_TerminalInitialize();
	GDT_Init();
	Interrupt_InstallISRs();
	__asm__  ("div %0" :: "r"(0));

	return 0;
}
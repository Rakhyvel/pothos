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

int sema;

int main() 
{
	sema = 1;
	VGA_TerminalInitialize();
	GDT_Init();
	Interrupt_InstallISRs();
	Timer_Init(60);
	//Keyboard_Init();

	while (1) {
		//printf("H\n");
	}

	return 0;
}
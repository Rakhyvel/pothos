/*	kernel.c

	The kernel's main function is called by boot.asm. This starts the OS

	Author: Joseph Shimel
	Date: 7/25/21
*/

#include "../devices/vga.h"

int main() 
{
	VGA_TerminalInitialize();
 
	char* str = "Hello, world!\n";
	for(int i = 0; i < 26; i++) {
		str[4] = i + '0';
		VGA_TerminalWriteString(str);
	}
	return 0;
}
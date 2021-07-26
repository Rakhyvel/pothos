#include "../devices/vga.h"

int main() 
{
	/* Initialize terminal interface */
	VGA_TerminalInitialize();
 
	/* Newline support is left as an exercise. */
	VGA_TerminalWriteString("Hello, World!");
	return 0;
}
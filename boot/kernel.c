#include "../devices/vga.h"

int main() 
{
	/* Initialize terminal interface */
	VGA_TerminalInitialize();
 
	/* Newline support is left as an exercise. */
	char* str = "Hello, world!\n";
	for(int i = 0; i < 26; i++) {
		str[4] = i + '0';
		VGA_TerminalWriteString(str);
	}
	return 0;
}
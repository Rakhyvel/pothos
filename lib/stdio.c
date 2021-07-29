#include "./stdio.h"
#include "./string.h"
#include "../devices/vga.h"

void printf(const char* msg) {
    int size = strlen(msg);
	for (size_t i = 0; i < size; i++) {
		VGA_TerminalPutChar(msg[i]);
	}
}
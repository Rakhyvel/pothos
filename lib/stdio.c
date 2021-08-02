/*  Author: Joseph Shimel
    Date:   7/26/21 */
    
#include "./stdio.h"
#include "./string.h"
#include "../devices/vga.h"

/*	Prints out a message to the screen

	@param msg	Message to print to screen */
void printf(char* msg) {
    int size = strlen(msg);
	for (size_t i = 0; i < size; i++) {
		VGA_TerminalPutChar(msg[i]);
	}
}
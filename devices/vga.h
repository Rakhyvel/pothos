/*	vga.h

	Author: Joseph Shimel
	Date: 7/25/21
*/

#ifndef _DEVICES_VGA
#define _DEVICES_VGA

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
 
/* Hardware text mode color constants. */
enum VGA_Color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GREY = 7,
	VGA_COLOR_DARK_GREY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_LIGHT_BROWN = 14,
	VGA_COLOR_WHITE = 15,
};
 
static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
 
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

uint16_t VGA_Entry(unsigned char uc, uint8_t color);
void VGA_Init(void);
void VGA_TerminalSetColor(uint8_t color);
void VGA_TerminalPutEntryAt(char c, uint8_t color, size_t x, size_t y);
void VGA_TerminalPutChar(char c);

#endif
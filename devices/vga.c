#include "./vga.h"
#include "../lib/string.h"

static inline uint8_t VGA_EntryColor(enum VGA_Color fg, enum VGA_Color bg) 
{
	return fg | bg << 4;
}
 
static inline uint16_t VGA_Entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
void VGA_TerminalInitialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = VGA_EntryColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xB8000;
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = VGA_Entry(' ', terminal_color);
		}
	}
}
 
void VGA_TerminalSetColor(uint8_t color) 
{
	terminal_color = color;
}
 
void VGA_TerminalPutEntryAt(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = VGA_Entry(c, color);
}
 
void VGA_TerminalPutChar(char c) 
{
	VGA_TerminalPutEntryAt(c, terminal_color, terminal_column, terminal_row);
	if (++terminal_column == VGA_WIDTH) {
		terminal_column = 0;
		if (++terminal_row == VGA_HEIGHT)
			terminal_row = 0;
	}
}
 
void VGA_TerminalWrite(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		VGA_TerminalPutChar(data[i]);
}
 
void VGA_TerminalWriteString(const char* data) 
{
	VGA_TerminalWrite(data, strlen(data));
}
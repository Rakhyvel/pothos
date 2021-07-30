/*	vga.c

	Contains some code for working with VGA in text mode

	Author: Joseph Shimel
	Date: 7/25/21
*/

#include "./vga.h"
#include "../lib/string.h"
#include "../devices/io.h"

static inline uint8_t VGA_EntryColor(enum VGA_Color fg, enum VGA_Color bg) 
{
	return fg | bg << 4;
}
 
inline uint16_t VGA_Entry(unsigned char uc, uint8_t color) 
{
	return (uint16_t) uc | (uint16_t) color << 8;
}
 
void VGA_Init(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = VGA_EntryColor(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	terminal_buffer = (uint16_t*) 0xC00B8000;
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

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;
 
	IO_OutByte(0x3D4, 0x0F);
	IO_OutByte(0x3D5, (uint8_t) (pos & 0xFF));
	IO_OutByte(0x3D4, 0x0E);
	IO_OutByte(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}
 
void VGA_TerminalPutChar(char c) 
{
    if(c == '\n') {
        ++terminal_row;
        terminal_column = 0;
    } else {
		const size_t index = terminal_row * VGA_WIDTH + terminal_column;
		terminal_buffer[index] = (uint16_t) c | (uint16_t) terminal_color << 8;
        if (++terminal_column > VGA_WIDTH) {
			++terminal_row;
            terminal_column = 0;
        }
    }
	if(terminal_row > VGA_HEIGHT) {
		memcpy(terminal_buffer, terminal_buffer + VGA_WIDTH, 4 * VGA_WIDTH * VGA_HEIGHT - 2);
		terminal_row--;
	}
	update_cursor(terminal_column, terminal_row);
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
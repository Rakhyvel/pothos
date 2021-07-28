#include "./timer.h"
#include "../kernel/interrupt.h"
#include "../devices/io.h"

#include "../lib/stdio.h"
#include "../lib/string.h"
#include "../kernel/kernel.h"
#include "../devices/vga.h"

int ticks;

void TerminalPutEntryAt(char c, uint8_t color, size_t x, size_t y) 
{
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = VGA_Entry(c, color);
}

/*
    If you call printf from this function, it raises a page fault
    Perhaps I just live with it? Maybe its because its an interrupt? */
void Timer_callback(registers_t regs) {
    ticks++;
	sema = !sema;
	VGA_TerminalPutEntryAt('X', VGA_COLOR_WHITE, 2, 2);
}

void Timer_Init(uint32_t freq) {
    ticks = 0;
	Interrupt_RegisterIRQHandler(IRQ0, &Timer_callback);

	uint32_t divisor = 119180 / freq;
	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
	
	// Send command
	IO_OutByte(0x43, 0x36); // Command port
	IO_OutByte(0x40, low);
	IO_OutByte(0x40, high);
}
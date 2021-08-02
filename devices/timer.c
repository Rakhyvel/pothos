/*  Author: Joseph Shimel
    Date:   7/26/21 */
    
#include "./timer.h"
#include "../kernel/idt.h"
#include "../kernel/interrupt.h"
#include "../devices/io.h"

#include "../lib/stdio.h"
#include "../lib/string.h"

int ticks;

/*	Handles timer interrupts

	@param regs	State of the registers at the time of the interrupt. Unused */
void Timer_callback(registers_t* regs) {
    ticks++;
}

/*	Initializes the timer interrupt handler.

	@param freq	The frequency that the timer should send interrupts at */
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
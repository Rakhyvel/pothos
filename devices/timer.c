#include "./timer.h"
#include "../threads/interrupt.h"
#include "../threads/io.h"

#include "../lib/stdio.h"

uint32_t ticks = 0;

static void Timer_callback(registers_t regs) {
    ticks++;
}

void Timer_Init(uint32_t freq) {
    // Install the function we just wrote
	Interrupt_RegisterIRQHandler(IRQ0, Timer_callback);

	uint32_t divisor = 119180 / freq;
	uint8_t low = (uint8_t)(divisor & 0xFF);
	uint8_t high = (uint8_t)((divisor >> 8) & 0xFF);
	
	// Send command
	IO_OutByte(0x43, 0x36); // Command port
	IO_OutByte(0x40, low);
	IO_OutByte(0x40, high);
}
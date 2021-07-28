#include "./io.h"

uint8_t IO_InByte (uint16_t port) {
    uint8_t result;
	// Run in instruction, "d" is the port, "a" is the result
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void IO_OutByte (uint16_t port, uint8_t data) {
	// send out byte to port
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

uint16_t IO_HalfIn (uint16_t port) {
    uint16_t result;
	// Run in instruction, "d" is the port, "a" is the result
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

void IO_HalfOut (uint16_t port, uint16_t data) {
	// send out byte to port
	__asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
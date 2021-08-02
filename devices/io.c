/*  Author: Joseph Shimel
    Date:   7/26/21 */

#include "./io.h"

/*  Reads in a byte from an IO port

    @param port port to read in from
    @return     byte at port */
uint8_t IO_InByte (uint16_t port) {
    uint8_t result;
	// Run in instruction, "d" is the port, "a" is the result
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

/*  Sends out a byte to an IO port

    @param port Port to send data out to
    @param data Data byte to send out */
void IO_OutByte (uint16_t port, uint8_t data) {
	// send out byte to port
	__asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

/*  Reads in a 16 bit half-word

    @param port port to read in from
    @return     data at port */
uint16_t IO_HalfIn (uint16_t port) {
    uint16_t result;
	// Run in instruction, "d" is the port, "a" is the result
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    return result;
}

/*  Sends out a 16-bit half-word

    @param port Port to write out to
    @param data Data to write */
void IO_HalfOut (uint16_t port, uint16_t data) {
	// send out byte to port
	__asm__("out %%ax, %%dx" : : "a" (data), "d" (port));
}
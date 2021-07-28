#ifndef _THREADS_IO_H
#define _THREADS_IO_H

#include <stddef.h>
#include <stdint.h>

uint8_t IO_InByte (uint16_t port);
void IO_OutByte (uint16_t port, uint8_t data);
uint16_t IO_HalfIn (uint16_t port);
void IO_HalfOut (uint16_t port, uint16_t data);

#endif
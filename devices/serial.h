#ifndef SERIAL_H
#define SERIAL_H

#include "../lib/system.h"

#define PORT_COM1 0x3f8

void qemu_printf(const char* s, ...);

#endif
#include "serial.h"
#include <stdarg.h>
#include "../lib/system.h"
#include "io.h"

int is_transmit_empty() {
   return IO_InByte(PORT_COM1 + 5) & 0x20;
}

void write_serial(char a) {
   while (is_transmit_empty() == 0);
   IO_OutByte(PORT_COM1, a);
}

void qemu_printf(const char* s, ...) {
    va_list(ap);
    va_start(ap, s);
    vsprintf(NULL, write_serial, s, ap);
    va_end(ap);
}
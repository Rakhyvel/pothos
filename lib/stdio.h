#ifndef _LIB_STDIO_H
#define _LIB_STDIO_H

#include <stdarg.h>

void vsprintf(char * str, void (*putchar)(char), const char * format, va_list arg);

void vsprintf_helper(char * str, void (*putchar)(char), const char * format, uint32_t * pos, va_list arg);

void printf(const char * s, ...);

#endif
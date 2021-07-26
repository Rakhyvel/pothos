/*	string.h

	Author: Joseph Shimel
	Date: 7/25/21
*/

#ifndef _LIB_STRING_H
#define _LIB_STRING_H

#include <stddef.h>
 
size_t strlen(const char* str);
void *memcpy(void *dest, const void *src, size_t n);

#endif
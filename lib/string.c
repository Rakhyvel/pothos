/*	string.c

	Author: Joseph Shimel
	Date: 7/25/21
*/

#include "./string.h"

size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

void* memcpy(void *dst_, const void *src_, size_t size)
{
	unsigned char *dst = dst_;
	const unsigned char *src = src_;

	if (dst < src) {
    	while (size-- > 0)
        	*dst++ = *src++;
    } else {
    	dst += size;
    	src += size;
    	while (size-- > 0)
    	*--dst = *--src;
    }
	return dst;
}
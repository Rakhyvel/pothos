/*	string.c

	Author: Joseph Shimel
	Date: 7/25/21
*/

#include "./string.h"

/*	Finds the length of a null terminated string

	@param str	String to find length of
	@return		Length of string */
size_t strlen(const char* str) 
{
	size_t len = 0;
	while (str[len])
		len++;
	return len;
}

char *strcpy(char *dest, const char *src) {
	int length = strlen(dest);
	memcpy(dest, src, length);
}

/*	Copies memory from a source to a destination

	@param dst_	Pointer to destination
	@param src_	Pointer to source
	@param size	Size to copy
	@return		Pointer to dst+size */
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

/*	Sets a number of bytes in an array to a specified number

	@param s	Pointer to array start
	@param c	Byte to fill
	@param n	Number of bytes to fill
	@return		Pointer to array start (s) */
void *memset(char* s, int c, size_t n) {
	for(int i = 0; i < n; i++) {
		s[i] = c;
	}
	return s;
}

/*	Converts an integer into a string

	@param buf	String buffer
	@param n	Number to convert
	@param base	Base of number */
void itoa(char *buf, unsigned long int n, int base) {
    unsigned long int tmp;
    int i, j;

    tmp = n;
    i = 0;

    do {
        tmp = n % base;
        buf[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
    } while (n /= base);
    buf[i--] = 0;

    for (j = 0; j < i; j++, i--) {
        tmp = buf[j];
        buf[j] = buf[i];
        buf[i] = tmp;
    }
}

int atoi(char * string) {
    int result = 0;
    unsigned int digit;
    int sign;

    while (isspace(*string)) {
        string += 1;
    }

    /*
     * Check for a sign.
     */

    if (*string == '-') {
        sign = 1;
        string += 1;
    } else {
        sign = 0;
        if (*string == '+') {
            string += 1;
        }
    }

    for ( ; ; string += 1) {
        digit = *string - '0';
        if (digit > 9) {
            break;
        }
        result = (10*result) + digit;
    }

    if (sign) {
        return -result;
    }
    return result;
}

int isspace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}
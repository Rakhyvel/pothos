/*	string.h

	Author: Joseph Shimel
	Date: 7/25/21
*/

#ifndef _LIB_STRING_H
#define _LIB_STRING_H

#include <stddef.h>
 
size_t strlen(const char* str);
char *strcpy(char *dest, const char *src);
void *memcpy(void *dest, const void *src, size_t n);
void *memset(char *s, int c, size_t n);
void itoa(char *buf, unsigned long int n, int base);
int atoi(char * string);
int isspace(char c);

#endif
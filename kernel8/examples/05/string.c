#include "string.h"

// based on https://en.wikibooks.org/wiki/C_Programming/String_manipulation
void *memset(void *s, int c, size_t n)
{
	unsigned char *us = s;
	unsigned char uc = c;
	while (n-- != 0)
		*us++ = uc;
	return s;
}

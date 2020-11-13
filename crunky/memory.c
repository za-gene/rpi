#include <memory.h>

void *memcpy(void *dest, const void *src, size_t n)
{
	char *d = (char*)dest;
	const char *s = (const char*) src;
	if(n>0)
		while(n--) *d++ = *s++;
	return dest;
}

void *memset(void* s, int c, size_t n)
{
	unsigned char* s1 = s;
	while(n--) *s1++ = (unsigned char) c;
	return s;
}

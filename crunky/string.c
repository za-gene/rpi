#include <string.h>
#include <strings.h>

// based on https://en.wikibooks.org/wiki/C_Programming/String_manipulation
void *memset(void *s, int c, size_t n)
{
	unsigned char *us = s;
	unsigned char uc = c;
	while (n-- != 0)
		*us++ = uc;
	return s;
}
	
void bzero(void *s, size_t n)
{
	memset(s, 0, n);
}

char *strcpy(char *dest, const char *src)
{
	while((*dest++ = *src++));
	return dest;
}


void *memchr(const void *s, int c, size_t n)
{
    unsigned char *p = (unsigned char*)s;
    unsigned char *isCharFind = NULL;
    while((s!= NULL) && (n--))
    {
        if( *p != (unsigned char)c )
        {
            p++;
        }
        else
        {
            isCharFind = p;
            break;
        }
    }
    return isCharFind;
}

size_t strlen(const char *s)
{
    size_t n = 0;
    while(*s++) n++;
    return n;
}

// https://stackoverflow.com/questions/14367727/how-does-strchr-implementation-work
char *strchr(const char *s, int c) 
{
    if (s == NULL) {
        return NULL;
    }
    if ((c > 255) || (c < 0)) {
        return NULL;
    }
    int s_len;
    int i;
    s_len = strlen(s);
    for (i = 0; i < s_len; i++) {
        if ((char) c == s[i]) {
            return (char*) &s[i];
        }
    }
    return NULL;
}



#include <stdint.h>
#include <memory.h>

/*
 * source:
 * https://stackoverflow.com/questions/14367727/how-does-strchr-implementation-work
 */
char *strchr(const char *s, int c) 
{
	while (*s != (char) c) {
		if (!*s++) {
			return NULL;
		}
	}
	return (char *)s;
}


/*
 * source:
 * https://stackoverflow.com/questions/34873209/implementation-of-strcmp
 */
int strcmp(const char *s1, const char *s2 )
{
	int i = 0;
	int flag = 0;
	while (flag == 0)
	{
		if (s1[i] > s2[i])
		{
			flag = 1;
		}
		else if (s1[i] < s2[i])
		{
			flag = -1;
		}

		if (s1[i] == '\0')
		{
			break;
		}

		i++;
	}
	return flag;
}


size_t strlen(const char *s)
{
	size_t n = 0;
	while(*s++) n++;
	return n;
}

void *memcpy(void *dest, const void *src, size_t n)
{
#if 0 // GCC builtin doesn't seem to work
	return __builtin_memcpy(dest, src, n);
#else
	char *d = dest;
	while(n--) *d++ = *(char*)src++;
	return dest;
#endif
}

extern uintptr_t __heap_start__[];
extern uintptr_t __heap_end__[];


/* source:
 * https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2020/06/10/malloc_and_sbrk-Qowo?filter=page+has+dearakpughaz_i_usedsbrkfunctionasyouexpl-o5rT
 * sbrk() is in #include <unistd.h>
 */
void* _sbrk(int incr)
{
	static char *heap_end = (char*) __heap_start__;
	char *prev_heap_end;

	//puts("sbrk has been called");
	//printf("_sbrk:heap_end:%X:incr:%d\n", heap_end, incr);
	prev_heap_end = heap_end;
	if ((heap_end + incr) > (char*)__heap_end__) {
		//errno = ENOMEM;
		return ((void*)-1); // error - no more memory
	}
	heap_end += incr;

	return (void*) prev_heap_end;
}



extern uintptr_t __bss_start__[];
extern uintptr_t __bss_end__[];

/* Zero the BSS section 4-bytes at a time */
void zero_bss(void)
{
	uint32_t *memloc = (uint32_t*)__bss_start__;

	while (memloc < (uint32_t*)__bss_end__)
		*memloc++ = 0;
}



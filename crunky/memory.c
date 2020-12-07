#include <stdint.h>
#include <memory.h>

void *memcpy(void *dest, const void *src, size_t n)
{
	char *d = (char*)dest;
	const char *s = (const char*) src;
	if(n>0)
		while(n--) *d++ = *s++;
	return dest;
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



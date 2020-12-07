# malloc

There are 3 components that allow malloc to work:
* linker.ld - this reserves memory for the "heap"
* _sbrk() - defined in memory.c, which moves a marker from the start of the heap towards the end of the heap
* newlib_nano libray - which implements malloc and friends. 
It needs an implementation of sbrk(), called _sbrk(), because in and of itself, it has no idea how 
the heap has been implemented


## linker.ld

linker.ld is a specification as to how memory should be laid out. Pertinent to the heap, it looks like this:

```
SECTIONS
{ 
	... code ...

	... bss section ...

	.crunky_heap : {
		. = ALIGN(4);
		_sheap = .;
		__heap_start__ =  _sheap;
		. = . + 100000000; /* 100MB heap */
		. = ALIGN(4);
		_eheap = .;
		__heap_end__ = _eheap;
	}

	...
}
```

A Pi Zero has 512M of RAM. A Pi 3 has 1G of RAM. In .crunky_heap, a section of memory is carved out. 
In the code above, 100MB is reserved for "the heap". Arguably is should be larger. 

The values `__heap__start__` and `__heap__end__` are needed by `sbrk()` to delimit the start and end of the heap.


## sbrk()

`sbrk()` changes the location of the "program break". Conceptually, it is the current high-water mark of the heap usage. It starts at `__heap_start__` and shift up to `__heap_end__`. `sbrk()` can shrink as well as expand. 
It is usual to consider it as expanding. However, if the last chunk of allocated memory is freed by `malloc()`,
then it makes sense for the break to be reduced.

Here is how it may be implemented (in memory.c):

```
extern uintptr_t __heap_start__[];
extern uintptr_t __heap_end__[];


/* source:
 * https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2020/06
/10/malloc_and_sbrk-Qowo?filter=page+has+dearakpughaz_i_usedsbrkfunctionasyouexp
l-o5rT
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
```

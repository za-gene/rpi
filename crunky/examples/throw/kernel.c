#include <bcm2835.h>
#include <mini_uart.h>
#include <stdio.h>
#include <timers.h>
#include <string.h>
#include <errno.h>

#include <stdlib.h>
#include <basal.h>
#include <assert.h>
void __assert_func (const char * filename, int lineno, const char * str1, const char *str2)
{
	printf("assertion failed:%s:%d:%s():%s\n", filename, lineno, str1, str2);
	while(1);
}


#define HEAP_SIZE 100
//#define HEAP_SIZE 100000000
//__attribute__((section(".crunky_heap"))) char heap[HEAP_SIZE];
//char heap[HEAP_SIZE];

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



void* xmalloc(size_t size)
{
	void* addr = malloc(size);
	assert(addr);
	//printf("xmalloc:heap:%x:addr:%X\n", heap, addr);
	if((addr == NULL) && size) {
		printf("xmalloc:panic allocating %d bytes\n", size);
		while(1);
	}
	return addr;
}

void foo()
{
	static int xxx = 5;
	assert(xxx==5);
}

#include <stdint.h>
extern uintptr_t __bss_start__[];
extern uintptr_t __bss_end__[];

/* Zero the BSS section 4-bytes at a time */
void zero_bss(void)
{
    uint32_t *memloc = (uint32_t*)__bss_start__;

    while (memloc < (uint32_t*)__bss_end__)
        *memloc++ = 0;
}





void kernel_main(void)
{
	//zero_bss();

	uart_init_as_stdio(115200);

	puts("\nmalloc test");
	foo();
	char* str1 = xmalloc(100);
	char* str2 = xmalloc(100);
	//sbrk(10);
	strcpy(str1, "My string 1");
	strcpy(str2, "My string 2");
	printf("String 1 is '%s'\n", str1);
	printf("String 2 is '%s'\n", str2);
	int i=0;
	while(1) {
		const size_t size = 0x100000;
		char* oops = xmalloc(size);
		strcpy(oops, "hello");
		bzero(oops, 0);
		//free(oops);
		printf("i=%d, oops=%X\n", i++, oops);
		//printf("i=%d, \n", i++);
	}
	//}
puts("I will now hang");
while(1);
#if 0
uart_init_as_stdio(115200);
puts("bcm spi master example");
bcm2835_init();
bcm2835_spi_begin();

// the following are defaults
bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_65536);
bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);

while (1) {
	// Send a byte to the slave and simultaneously read a byte back from the slave
	// If you tie MISO to MOSI, you should read back what was sent
	uint8_t data = bcm2835_spi_transfer(0x23); // 0x23 is just a random number
	printf("Read from SPI: %d\n", data);
	delay_s(1);
}
#endif
}

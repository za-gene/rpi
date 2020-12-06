#include <bcm2835.h>
#include <mini_uart.h>
#include <stdio.h>
#include <timers.h>
#include <string.h>
//#include <errno.h>

#include <stdlib.h>
#include <basal.h>

#define HEAP_SIZE 400000000
char heap[HEAP_SIZE]; // 400M of heap
int errno = 0;
#define ENOMEM 1

/* source:
 * https://www.silabs.com/community/mcu/32-bit/knowledge-base.entry.html/2020/06/10/malloc_and_sbrk-Qowo?filter=page+has+dearakpughaz_i_usedsbrkfunctionasyouexpl-o5rT
 * sbrk() is in #include <unistd.h>
 */
//const char* _end = heap;         
//char* __HeapLimit = heap + HEAP_SIZE;
void* sbrk(int incr)
{
	//static char *
	static char *heap_end = 0;
	char *prev_heap_end;


	if (heap_end == 0) {
		heap_end = heap;
	}


	puts("sbrk has been called");
	printf("_sbrk:heap_end:%X:incr:%d\n", heap_end, incr);
	prev_heap_end = heap_end;
	//if ((heap_end + incr) > (char*) &__HeapLimit) {
	if ((heap_end + incr) > heap + HEAP_SIZE) {
		errno = ENOMEM;
		return ((void*)-1); // error - no more memory
	}
	heap_end += incr;

	return (void*) prev_heap_end;
}


/*
   class Foo {
   public:
   Foo() { puts("Foo created"); }
   ~Foo() { puts("Foo says bye") ;}
   };
   */

void foo()
{
	//Foo  x;
	//throw 12;
	volatile void* m = malloc(12);
}


void* xmalloc(size_t size)
{
	void* addr = malloc(size);
	printf("xmalloc:heap:%x:addr:%X\n", heap, addr);
	if((addr == NULL) && size) {
		printf("xmalloc:panic allocating %d bytes\n", size);
		while(1);
	}
	return addr;
}

void kernel_main(void)
{
	uart_init_as_stdio(115200);
	puts("\nmalloc test");
	char* str1 = xmalloc(100);
	char* str2 = xmalloc(100);
	sbrk(10);
	strcpy(str1, "My string 1");
	strcpy(str2, "My string 2");
	printf("String 1 is '%s'\n", str1);
	printf("String 2 is '%s'\n", str2);
	//while(1) {
	volatile void* oops = xmalloc(HEAP_SIZE);
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

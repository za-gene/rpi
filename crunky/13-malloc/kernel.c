//#include <mini_uart.h>
//#include <uart0.h>
#include <stdio.h>
//#include <timers.h>
#include <string.h>

#include <stdlib.h>
#include <basal.h>
#include <assert.h>



void* xmalloc(size_t size)
{
	void* addr = malloc(size);
	assert(addr); // will eventually cause a panic. 94 steps in my case.
	//printf("xmalloc:heap:%x:addr:%X\n", heap, addr);
	if((addr == NULL) && size) {
		printf("xmalloc:panic allocating %d bytes\n", size);
		while(1);
	}
	return addr;
}


//#include <stdint.h>



void kernel_main(void)
{
	//mini_uart_init_as_stdio();
	//uart0_init_as_stdio();

	puts("\nmalloc test");
	char* str1 = xmalloc(100);
	char* str2 = xmalloc(100);
	strcpy(str1, "My string 1");
	strcpy(str2, "My string 2");
	printf("String 1 is '%s'\n", str1);
	printf("String 2 is '%s'\n", str2);

	puts("Let's try to exhaust memory");
	int i=0;
	while(1) {
		const size_t size = 0x100000;
		char* oops = xmalloc(size);
		strcpy(oops, "hello");
		bzero(oops, 0);
		printf("%d ", i++);
	}
}

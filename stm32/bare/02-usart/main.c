#include <string.h>
#include <stddef.h> // for size_t
#include <stdio.h>
#include "../blue.h"

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;

char greeting[] = "Hello from bare metal usart 8";

void* memcpy_usr(void* dst, const void* src, size_t n) {
	// Copies n bytes from src to dst

	// Since we cannot dereference a void* ptr,
	// we first typecast it to a char* ptr
	// and then do the copying byte by byte,
	// since a char* ptr references a single byte
	char* char_dst = (char*) dst;
	char* char_src = (char*) src;

	for (int i=0; i<n; i++) {
		*char_dst++ = *char_src++;
	}

	return dst;
}

void *memset_usr(void *b, int c, int len)
{
	int           i;
	unsigned char *p = b;
	i = 0;
	while(len > 0)
	{
		*p = c;
		p++;
		len--;
	}
	return(b);
}

void init_mem()
{
	// not called explicitly, but done in crt.s prior to calling main()

	// Copy initialized data from .sidata (Flash) to .data (RAM)
	memcpy_usr( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
	// Clear the .bss section in RAM.
	memset_usr( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );
}

void main() 
{
	init_serial();
	// Main loop: wait for a new byte, then echo it back.
	//char rxb = '\0';
	putchar('\a'); // beep
	puts(greeting);
	puts("You type, I echo.");
	while ( 1 ) {
		int c = getchar();
		putchar(c);
	}
}

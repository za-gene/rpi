/**
 * @file
 * @brief yakkity yak todo
 */
 
#include <stdbool.h>
#include "blue.h"

extern uint32_t _sidata, _sdata, _edata, _sbss, _ebss;


void* memcpy(void* dst, const void* src, size_t n) {
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

void *memset(void *b, int c, int len)
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
	memcpy( &_sdata, &_sidata, ( ( void* )&_edata - ( void* )&_sdata ) );
	// Clear the .bss section in RAM.
	memset( &_sbss, 0x00, ( ( void* )&_ebss - ( void* )&_sbss ) );
}


void put32(u32 addr, u32 val)
{
	*(volatile u32*) addr = val;
}

u32 get32(u32 addr)
{
	return *(volatile u32*) addr;
}

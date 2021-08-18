/* infrastructural support */

#include <stdio.h>

#include <basal.h>

//#include <tinyprintf.h>


const u32 apb_clock_freq = 250000000; // 250MHz Advanced Peripheral Bus clock freq. Pi 3
int __errno = 0;



/** setup stuff that gets called before your kernel
 */

extern void zero_bss(void);
extern void uart0_init_as_stdio(void);
extern void bcm2835_init(void);

void premain(void)
{
	zero_bss();
	uart0_init_as_stdio();
	bcm2835_init();
}


///////////////////////////////////////////////////////////////////////
// MEMORY STUFF

void *memset(void* s, int c, size_t n)
{
	while(n--) *((uint8_t*) s+n) = c;
	return s;
}

void bzero(void* s, size_t n)
{
	memset(s, 0, n);
}

char* strcpy(char* dest, const char* src)
{
	char* ret = dest;
	while(*dest++ = *src++);
	return ret;
}


/* Note that this is a seriously underspecified implementation.
 * You can't free memory, for example
 */
void *malloc(size_t size)
{
	extern void* _sbrk(int incr);
	void* addr =  _sbrk(size);
	if(addr == (void*)-1) return 0;
	return addr;
}

void free(void *ptr)
{
	// distinctly lacking!
}

/* taken from
 * https://stackoverflow.com/questions/32560167/strncmp-implementation
 */
int strncmp( const char * s1, const char * s2, size_t n )
{
	while ( n && *s1 && ( *s1 == *s2 ) )
	{
		++s1;
		++s2;
		--n;
	}
	if ( n == 0 )
	{
		return 0;
	}
	else
	{
		return ( *(unsigned char *)s1 - *(unsigned char *)s2 );
	}
}


///////////////////////////////////////////////////////////////////////
// STDIO



fn_putchar basal_putchar = 0;

void set_putchar(fn_putchar fn)
{
	basal_putchar = fn;
}



int putchar(int c)
{
	if(!basal_putchar) return EOF;
	if(c == '\n') basal_putchar('\r');
	return basal_putchar(c);
}



fn_getchar basal_getchar = 0; // _getchar_nada;

int getchar(void)
{

	if(!basal_getchar) return EOF;
	return basal_getchar();
}

void set_getchar(fn_getchar fn)
{
	basal_getchar = fn;
}


/*
   int getchar()
   {
   return _getchar();
   }
   */


int newline(void)
{
	return putchar('\n');
}

int print_string(const char *s)
{
	if(!s) return EOF;
	while(*s) {
		int ok = putchar(*s++);
		if(ok == EOF) return EOF;
	}
	return 1;
}


int puts(const char *s)
{
	print_string(s);
	newline();
	return 1;
}



///////////////////////////////////////////////////////////////////////
// MATHS

#if 1
// ARM EABI unsigned integer division bit modulus support for GCC
u64 __aeabi_uidivmod(u32 value, u32 divisor)
{
	u64 answer = 0;
	int bit;

	// Bitwise divide algorithm, loop MSB order for 32 bits
	for (bit = 31; bit >= 0; --bit)
	{
		// If this bit is set in the divisor
		if ((divisor << bit) >> bit == divisor)
		{
			if (value >= divisor << bit)
			{
				value -= divisor << bit;
				answer |= 1 << bit;
				if (value == 0)
					break;
			}
		}
	}

	// Add remainder into the high order bits before returning answer
	answer |= (u64)value << 32;
	return answer;
}

u64 __aeabi_uldivmod(u64 value, u64 divisor)
{
	u64 answer = 0;
	int bit;

	// Bitwise divide algorithm, loop MSB order for 32 bits
	for (bit = 31; bit >= 0; --bit)
	{
		// If this bit is set in the divisor
		if ((divisor << bit) >> bit == divisor)
		{
			if (value >= divisor << bit)
			{
				value -= divisor << bit;
				answer |= 1 << bit;
				if (value == 0)
					break;
			}
		}
	}

	// Add remainder into the high order bits before returning answer
	answer |= (u64)value << 32;
	return answer;
}

// ARM EABI unsigned integer division support for GCC
u32 __aeabi_uidiv(u32 value, u32 divisor)
{
	// Use the division modulus, ignoring/truncating the remainder
	return __aeabi_uidivmod(value, divisor);
}

// ARM EABI unsigned integer division support for GCC
int __aeabi_idiv(int value, int divisor)
{
	// Use the division modulus, ignoring/truncating the remainder
	return __aeabi_uidivmod(value, divisor);
}

int __aeabi_idivmod(int value, int divisor)
{
	return value - (value/divisor)*divisor;
}

#endif

/* infrastructural support */

#include <stdio.h>

#include <basal.h>
//#include <tinyprintf.h>


const u32 apb_clock_freq = 250000000; // 250MHz Advanced Peripheral Bus clock freq. Pi 3



///////////////////////////////////////////////////////////////////////
// ASSERTIONS
void __assert_func (const char * filename, int lineno, const char * funcname, const char *expr)
{
	printf("assertion failed:%s:%d:%s():%s\n", filename, lineno, funcname, expr);
	while(1);
}


///////////////////////////////////////////////////////////////////////
// STDIO



// an implementation that does nothing
int _putchar_nada(int c)
{
	return EOF;
}

fn_putchar _putchar = _putchar_nada;

void set_putchar(fn_putchar fn)
{
	_putchar = fn;
}



// an implementation that does nothing
int _getchar_nada()
{
	return EOF;
}


fn_getchar _getchar = _getchar_nada;

void set_getchar(fn_getchar fn)
{
	_getchar = fn;
}




int newline()
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


#endif

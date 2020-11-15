/* infrastructural support */

#include <stdint.h>

typedef uint32_t u32;
typedef uint64_t u64;

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

// ARM EABI unsigned integer division support for GCC
u32 __aeabi_uidiv(u32 value, u32 divisor)
{
  // Use the division modulus, ignoring/truncating the remainder
  return __aeabi_uidivmod(value, divisor);
}


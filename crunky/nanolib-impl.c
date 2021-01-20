/* mcarter 2021-01-20 added
 * implementation functions that may be required by nanolib
 */

#include <stdio.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/types.h>
#include <unistd.h>

#include <basal.h>




int _open(const char *pathname, int flags)
{
	static int fd = 0;
	return fd++;
}



int _kill(pid_t pid, int sig)
{
	return 0;
}


pid_t _getpid(void)
{
	return 0;
}


clock_t _times(struct tms *buf)
{
	return 0;
}



int _unlink(const char *pathname)
{
	return 0;
}


ssize_t _write(int fd, const void *buf, size_t count)
{
	char* b = (char*) buf;
	for(int i = 0; i<count; i++)
		putchar(b[i]);
	return count;
}



int _close(int fd)
{
	return 0;
}


// just a dummy
int _fstat(int fd, struct stat *statbuf)
{
	return 0;
}



// just a dummy
int _gettimeofday(struct timeval *tv, struct timezone *tz)
{
	return 0;
}


void  _exit(int status)
{
	for(;;);
}
/*
   double difftime(time_t time1, time_t time0)
   {
   return (double)(time1-time0);
   }
   */

ssize_t _read(int fd, void *buf, size_t count)
{
	ssize_t i = 0;	
	//while(count--)
	//	*buf = (char) getchar();
	char* b = (char*) b;
	while(i<count)
		b[i++] = (char) getchar();
		//*(buf+ i++) =(char)  getchar();
	return count;
}

// just a dummy
off_t _lseek(int fd, off_t offset, int whence)
{
	return offset;
}


// just a dummy
int _link(const char *oldpath, const char *newpath)
{
	return 0;
}


// just a dummy
int _isatty(int fd)
{
	return 1;
}




///////////////////////////////////////////////////////////////////////
// MATHS


/* Shouldn't be required if you link against libgcc
 */
#if 0
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

//__aeabi_idiv(

#endif

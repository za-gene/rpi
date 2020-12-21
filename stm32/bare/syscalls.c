/**
 * Implements functions required by newlib nano
 *
 * errno implementation is a bit of a fudge
 * See also:
 * /usr/include/newlib/sys/errno.h
 */

#include <errno.h> // ENOMEM
//#include <stdlib.h> // for caddr_t
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

#undef errno
int errno;
/*
int *__errno()
{
	return &errno;
}
*/

// these symbols defined in linker.ld
extern char _sheap;
extern char _eheap;

caddr_t _sbrk(int incr)
{
	static char *heap_end = 0;

	if (heap_end == 0) {
		heap_end = (char*) &_sheap;
	}

	char* prev_heap_end = heap_end;
	if((heap_end + incr) > (char*) &_eheap) {
		errno = ENOMEM; 
		return ((void*)-1); // error - no more memory
	}
	heap_end += incr;

	return (caddr_t) prev_heap_end;
}

int _open(const char *path , int flags , ...) 
{
	return 1;
}

int _close(int fd)
{
	return 0;
}

int _fstat(int fd , struct stat *st) 
{
	st->st_mode = S_IFCHR;
	return 0;
}

int _isatty(int fd)
{
	return 1;
}

int _lseek(int fd, off_t pos, int whence )
{
	return -1;
}

int _read(int fd, char *buf, size_t cnt)
{
	//*buf = getch();
	*buf = getchar();

	return 1;
}

int _write(int file, char *ptr, int len)
{
	for(int i=0; i < len; i++) 
	   putchar(*ptr++);
	
	return len;
}

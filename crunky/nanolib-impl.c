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


extern fn_putchar basal_putchar;
extern fn_getchar basal_getchar;

void __assert_func (const char * filename, int lineno, const char * funcname, const char *expr)
{
	printf("assertion failed:%s:%d:%s():%s\n", filename, lineno, funcname, expr);
	while(1);
}



int _open(const char *pathname, int flags)
{
	puts("_open called");
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
	//puts("_write called");
	char* b = (char*) buf;
	for(int i = 0; i<count; i++)
		basal_putchar(b[i]);
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

/** _read() would normally be called if we hadn't specified getchar().
 * We need to specify getchar() and not let read() handle it because 
 * newlib will keep calling until a newline, which is not what we want
 */

ssize_t _read(int fd, void *buf, size_t count)
{
	//printf("read called with fd %d, count %d\n", fd, count);
	//printf("zass 1\n");
	ssize_t i = 0;	
	//printf("zass 2\n");
	//while(count--)
	//	*buf = (char) getchar();
	char* b = (char*) buf;
	//printf("zass 3\n");
	size_t nread = 0;
	//printf("zass 4\n");

	if(count==0) return 0;

	// call to stdin should jusst return 1 char
	b[0] = basal_getchar();
	return 1;
	while(nread<count) {
		printf("X");
		char c= basal_getchar();
		printf(".");
		b[nread++] = c;
		if(c=='\n') break;
		//nread++;
		//*(buf+ i++) =(char)  getchar();
	}
	puts("exiting _read");
	return nread;
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




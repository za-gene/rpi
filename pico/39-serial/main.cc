#include <assert>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int main()
{

	int fd1=open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);
	assert(fd1 != -1);
	fcntl(fd1, F_SETFL,0);

	while(1) {
	char c;
	int rd = read(fd1, &c, 1);
	putchar(c);
	}

	close(fd1);
	return 0;

}

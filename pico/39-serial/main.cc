#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int main()
{

	//int fd1=open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
	int fd1=open("/dev/ttyACM0", O_RDWR | O_NOCTTY);
	assert(fd1>=0);

	struct termios tty;
	if(tcgetattr(fd1, &tty) != 0) {
		printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
	}
	cfsetspeed(&tty, B115200);
	//fcntl(fd1, F_SETFL,0);
	cfmakeraw(&tty); // make it a raw tty, which fixes \n issues that I was having before

	// Save tty settings, also checking for error
	if (tcsetattr(fd1, TCSANOW, &tty) != 0) {
		printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
		return 1;
	}

	while(1) {
		char c;
		int rd = read(fd1, &c, 1);
		//if(rd>0) 
		//	printf("%d \n", c);
		putchar(c);
	}

	close(fd1);
	return 0;

}

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <time.h>

using std::cout;
using std::string;

void pause(int fd)
{
		tcdrain(fd); // wait until transmission complete. needed-?
		usleep(1 * 1000); // delay n ms
}

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

	std::ifstream input("lorem.txt");
	std::stringstream sstr;
	while(input >> sstr.rdbuf());
	//cout << "size = " << sstr.str().size() << "\n";
	char cmd = 'T';
	write(fd1, &cmd, 1);
	//string& str = sstr.str();
	uint32_t size = sstr.str().size();
	write(fd1, &size, 4);
	tcdrain(fd1); // wait until transmission complete. needed-?
	for(int i = 0; i<size; i++) {
		char c = sstr.str()[i];
		//putchar(c);
		write(fd1, &c, 1);
		pause(fd1);
	}


	// write it back out again
	puts("Checking what we've received");
	cmd = 'R';
	write(fd1, &cmd, 1);
	pause(fd1);
	uint32_t rxsize;
	read(fd1, &rxsize, 4);
	cout << "rxsize = " << rxsize << "\n";
	for(int i = 0; i<rxsize; i++) {
		char c;
		//pause(fd1);
		int rd = read(fd1, &c, 1);
		putchar(c);
	}



	/*
	std::cout << sstr.str() << std::endl;
	char block[512];
	FILE* fin = fopen("lorem.txt", "r");
	assert(fin);
	while(1) {

		char c;
		int rd = read(fd1, &c, 1);
		//if(rd>0) 
		//	printf("%d \n", c);
		putchar(c);
	}
*/
	close(fd1);
	return 0;

}

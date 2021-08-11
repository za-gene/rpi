#include <stdio.h>
#include <unistd.h>

#include "crc8.h"

int main()
{
	unsigned char buff[512];
	unsigned char crc = 0;
	while(1) {
		ssize_t n = read(STDIN_FILENO, buff, 512);
		if(n == 0) break;
		crc = crc8_dallas_chunk(crc, buff, n);
		if(n<512) break;
	}
	printf("%d\n", (int) crc);

}

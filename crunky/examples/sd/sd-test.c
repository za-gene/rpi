#include <stdio.h>
#include <string.h>

#include "sd.h"

void sd_test()
{
	unsigned char buffer[512];
	sd_init();
	sd_readblock(614400,  buffer, 1);
	buffer[17] = 0;
	puts((char*) buffer);

	char output[] = "RASPBERRY PI WRITEBACK";
	memcpy(buffer, output, strlen(output));
	sd_writeblock(buffer, 614400, 1);

}

#include <lfb.h>
#include <stdio.h>

extern void sd_test();

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

	char output[] = "RASPBERRY PI 3 WRITEBACK";
	strcpy(buffer, output);
	sd_writeblock(buffer, 614400, 1);

}
void kernel_main()
{
	lfb_init_as_stdout();

	sd_test();

	puts("That's me done");
	puts("I'm just going to hang now");

	while(1);

}

#include <lfb.h>
#include <stdio.h>
#include <string.h>

#include <sd.h>
#include <uart0.h>

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
	//lfb_init_as_stdout();
	uart0_init_as_stdio();
	sd_test();

	puts("That's me done");
	puts("I'm just going to hang now");

	while(1);

}

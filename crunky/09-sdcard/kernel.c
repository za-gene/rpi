#include <stdio.h>
#include <string.h>
#include <timers.h>
#include <sdcard-rpi.h>

#include "../pico/fat32.h"


void sd_test1(void)
{
	unsigned char buffer[512];
	//sd_init();
	//sd_readblock(614400,  buffer, 1);
	buffer[17] = 0;
	puts((char*) buffer);

	char output[] = "RASPBERRY PI 3 WRITEBACK";
	strcpy(buffer, output);
	//sd_writeblock(buffer, 614400, 1);

}
void sd_test2(void)
{
	fat32_init();
	fat32_type_partition_table();
	fat32_list_root();
}


void kernel_main()
{
	puts("\nsdcard speed test");

	hare_start("timer test");
	delay_s(1);
	hare_stop();

	hare_start("sd_init");
	sdcard_init();
	hare_stop();

	u8 buffer[4096];

	hare_start("read 1 block");
	readablock(0, buffer);
	hare_stop();

	hare_start("read 4096 bytes");
	sd_readblock(0, buffer, 4096/512);
	hare_stop();

	puts("That's me done");
	puts("I'm just going to hang now");
}

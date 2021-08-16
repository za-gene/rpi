#include <stdio.h>
#include <string.h>
#include <timers.h>
#include <sdcard-rpi.h>

#include "../pico/crc8.h"
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

void sd_test3(void)
{
	fat32_init();
	char outfile[12];
	canfile(outfile, "song.raw");
	file32_t file;
	file32_init(&file, outfile);
	//File file(outfile);
	if(!file32_found(&file)) puts("ERR: file not found");
	uint8_t block[512];
	int n;
	puts("");
	hare_start("reading song.raw");
	while(n = file32_read(&file, block)); // just read, don't do anything
	hare_stop();
	printf("filesize: %d\n", file32_size(&file));
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

	int crc = crc8_dallas(buffer, 4096);
	printf("crc8 of last read is %d\n", crc);

	sd_test3();

	puts("That's me done");
	puts("I'm just going to hang now");
}

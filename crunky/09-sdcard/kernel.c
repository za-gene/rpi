#include <stdio.h>
#include <string.h>

#include "../pico/fat32.h"


void sd_test()
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
void kernel_main()
{
	fat32_init();
	fat32_type_partition_table();

	//printf("press any key to get the contents of readme.txt");
	//getchar();
	fat32_list_root();

	puts("That's me done");
	puts("I'm just going to hang now");
}

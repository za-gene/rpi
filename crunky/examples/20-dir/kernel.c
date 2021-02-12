#include <mini_uart.h>
#include <stdio.h>
#include <sd.h>
#include <fat.h>

void kernel_main(void)
{
	//uart_init(9600);
	uart_init_as_stdio(115200);
	// initialize EMMC and detect SD card type
	if(sd_init()==SD_OK) {
		// read the master boot record and find our partition
		if(fat_getpartition()) {
			// list root directory entries
			fat_listdirectory();
		} else {
			puts("FAT partition not found???\n");
		}
	}

	puts("Done. Hanging.");
	while(1);
}

//#include <mini_uart.h>
#include <stdio.h>
//#include <sd.h>
#include <../pico/fat32.h>


#if 0
void uart_send_1(char c) { putchar(c); }

/**
 * Display a binary value in hexadecimal
 */
void uart_hex_1(unsigned int d) {
	unsigned int n;
	int c;
	for(c=28;c>=0;c-=4) {
		// get highest tetrad
		n=(d>>c)&0xF;
		// 0-9 => '0'-'9', 10-15 => 'A'-'F'
		n+=n>9?0x37:0x30;
		uart_send(n);
	}
}

/**
 * Dump memory
 */
void uart_dump_1(void *ptr)
{
	unsigned long a,b,d;
	unsigned char c;
	for(a=(unsigned long)ptr;a<(unsigned long)ptr+512;a+=16) {
		uart_hex_1(a); puts(": ");
		for(b=0;b<16;b++) {
			c=*((unsigned char*)(a+b));
			d=(unsigned int)c;d>>=4;d&=0xF;d+=d>9?0x37:0x30;uart_send(d);
			d=(unsigned int)c;d&=0xF;d+=d>9?0x37:0x30;uart_send(d);
			uart_send_1(' ');
			if(b%4==3)
				uart_send_1(' ');
		}
		for(b=0;b<16;b++) {
			c=*((unsigned char*)(a+b));
			uart_send_1(c<32||c>=127?'.':c);
		}
		uart_send_1('\r');
		uart_send_1('\n');
	}
}


void kernel_mainXXX(void)
{
	unsigned int cluster;
	//uart_init(9600);
	//uart_init_as_stdio(115200);
	// initialize EMMC and detect SD card type
	if(sd_init()==SD_OK) {
		// read the master boot record and find our partition
		if(fat_getpartition()) {
			// find out file in root directory entries
			cluster=fat_getcluster("TEST    TXT");
			if(cluster==0)
				cluster=fat_getcluster("KERNEL  IMG");
			if(cluster) {
				// read into memory
				uart_dump_1(fat_readfile(cluster));
			}
		} else {
			puts("FAT partition not found???\n");
		}
	}


	puts("Done. Hanging.");
	while(1);
}
#endif

void kernel_main(void)
{
	puts("\nTyping file readme.txt ...");
	fat32_init();
	file32_type("readme.txt");
	puts("\n... Typing finished. Hanging.");
}

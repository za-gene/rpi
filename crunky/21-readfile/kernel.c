#include <lfb.h>
#include <stdio.h>
//#include <sd.h>
#include <../pico/fat32.h>


#if 0
void uart_send_1(char c) { putchar(c); }

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


#endif

void type_readme()
{
	puts("\nTyping file readme.txt ...");
	fat32_init();
	file32_type("readme.txt");
}

void kernel_main(void)
{
	//lfb_init_as_stdout();
	//type_readme();
	extern void sdcard_init(void);
	sdcard_init();
	puts("\n... Typing finished. Hanging.");
}

/* released into the Public Domain by mcarter 2020-12-04
*/

#include <basal.h>
#include <uart0.h>
#include <lfb.h>
#include <printf.h>
#include <yastdio.h>

#if 0
int uart0_putchar(int c)
{
	uart0_send((unsigned int) c);
	return c;
}
#endif

void kernel_main()
{
	char msg[] = "\nI'll now echo back everything you type 3";

	lfb_init();
	fbputs(msg);


	// display an ASCII string on screen with PSF
	//lfb_print(80, 80, "Hello World!");

	// display a UTF-8 string on screen with SSFN
	//lfb_proprint(80, 120, "Hello 多种语言 Многоязычный többnyelvű World!");


	uart0_init();
	set_putchar(uart0_putchar);
	puts("Calculate 203.56/10.5, then hang");
	//volatile u32 d = 203.56 / 10.5;
	volatile double d = 250.3e6 / 10.5;
	//uart0_hex(d);
	//printf("Computer says: %f\n", d);
	printf("printf says hello");
	puts("puts says hello");
	puts("Value put");

	while(1);

	// echo everything back
	while(1) {
		int c = uart0_getc();
		fbputchar(c);
		uart0_putchar(c);
	}
}

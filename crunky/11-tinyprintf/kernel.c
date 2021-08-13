/* released into the Public Domain by mcarter 2020-12-04
*/

#include <basal.h>
#include <mini_uart.h>
#include <stdio.h>
//#include <stdarg.h>
//#include <yastdio.h>
//#include <tinyprintf.h>


void kernel_main()
{
	mini_uart_init_as_stdio();
	//set_putchar(uart0_putchar);
	puts("\nCalculate 203.56/10.5 (=19.3867), then hang");
	volatile u32 d = 203.56 / 10.5;

	//char buf[100];
	printf("hello from sprintf. The meaning of life is: %10d\n", 42);
	printf("the integer value of the calc is: %d\n", d);
	double dd = 203.56 / 10.5;
	printf("as a double: %f\n", dd);
	puts("doesn't print a double, alas");
	//puts(buf);

	puts("HANGING");

	while(1);

}

/* released into the Public Domain by mcarter 2020-12-04
*/

#include <basal.h>
#include <uart0.h>
#include <stdio.h>
#include <stdarg.h>
#include <yastdio.h>
#include <tinyprintf.h>


void kernel_main()
{
	uart0_init();
	set_putchar(uart0_putchar);
	puts("\nCalculate 203.56/10.5, then hang");
	volatile u32 d = 203.56 / 10.5;

	char buf[100];
	tfp_snprintf(buf, sizeof(buf)-1, "hello from sprintf %10d nice", 42);
	puts(buf);

	puts("HANGING");

	while(1);

}

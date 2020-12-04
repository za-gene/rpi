/* released into the Public Domain by mcarter 2020-12-04
*/

#include <basal.h>
#include <uart0.h>
#include <lfb.h>
#include <stdio.h>
#include <stdarg.h>
#include <yastdio.h>
#include <tinyprintf.h>

#if 0
int uart0_putchar(int c)
{
	uart0_send((unsigned int) c);
	return c;
}
#endif

int prints(int n, ...)
{
  va_list va;
  va_start(va, n);
  for(int i = 0; i<n; i++) {
	  char* str = va_arg(va, char*);
	  puts(str);
  }
  va_end(va);
  return 1;
}


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
	puts("\nCalculate 203.56/10.5, then hang");
	volatile u32 d = 203.56 / 10.5;
	//volatile double d = 250.3e6 / 10.5;
	//uart0_hex(d);
	//tfp_snprintf("Computer says: %d\n", d);
	prints(3, "one", "two", "three");


	char buf[100];
	tfp_snprintf(buf, sizeof(buf)-1, "hello from sprintf %10d nice", 42);
	puts(buf);

	//printf("printf says hello");
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

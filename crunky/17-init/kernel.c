//#include <mini_uart.h>
#include <stdio.h>

static int var = 42;
int *p_var = &(var);

void kernel_main(void)
{
	//uart_init(9600);
	//uart_init_as_stdio(115200);
	puts("init test");
	printf("var=%d\n", *p_var);

	puts("I will now echo what you type");
	while (1) {
		char c = getchar();
		putchar(c);
		//if(c=='\r') uart_send('\n'); // prolly needs linefeed
	}
}

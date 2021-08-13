#include <stdio.h>
#include <uart0.h>
//#include <lfb.h>

void kernel_main()
{
    char msg[] = "I'll now echo back everything you type 1";

    uart0_init_as_stdio();
    //lfb_init();
    //fbputs(msg);

    // display an ASCII string on screen with PSF
    //lfb_print(80, 80, "Hello World!");

    // display a UTF-8 string on screen with SSFN
    //lfb_proprint(80, 120, "Hello 多种语言 Многоязычный többnyelvű World!");


    //uart0_init();
    //uart0_puts(msg);
    puts(msg);

    // echo everything back
    while(1) {
	    //int c = uart0_getc();
	    //puts("calling getchar...");
	    int c = getchar();
	    //puts("...done");
	    //fbputchar(c);
    	    //uart0_putchar(c);
    	    putchar(c);
    }
}

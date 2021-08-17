#include <lfb.h>
#include <basal.h>
#include <timers.h>
#include <stdio.h>


void kernel_main()
{
	lfb_init_as_stdout();

	// print a little triangle
	for(int i =0; i<8; i++) {
		for(int j = 0; j<i; j++) {
			putchar('*');
		}
		newline();
	}

	//puts("That's me done");
	//puts("I'm just going to hang now");

	int i = 0;
	while(i<20) {
		puts("*************************");
		printf("Count = %d\n", i++);
		delay_ms(250);
	}


	lfb_print_info();
	puts("I'm bored. Bye.");

}

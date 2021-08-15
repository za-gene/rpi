#include <lfb.h>
#include <basal.h>
#include <timers.h>
#include <stdio.h>

#define pinfo(x) printf(#x " = %d\n", x)

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

	pinfo(lfb_width());
	pinfo(lfb_height());
	pinfo(lfb_pitch());
	pinfo(lfb_pitch());
	pinfo(lfb_num_chars_in_col());
	pinfo(lfb_font_height());
	pinfo(lfb_font_width());

	int i = 0;
	while(i<20) {
		puts("*************************");
		printf("Count = %d\n", i++);
		delay_ms(250);
	}
	puts("I'm bored. Bye.");

}

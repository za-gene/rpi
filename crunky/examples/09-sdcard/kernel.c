#include <lfb.h>
#include <stdio.h>

extern void sd_test();

void kernel_main()
{
	lfb_init_as_stdout();

	sd_test();

	puts("That's me done");
	puts("I'm just going to hang now");

	while(1);

}

#include <lfb.h>
#include <stdio.h>
#include <string.h>

#include <sd.h>
#include <tat.h>
#include <uart0.h>


void kernel_main()
{
	uart0_init_as_stdio();
	puts("Initialising card");
	sd_init();
	puts("Mounting partition...");
	tat_mount();
	puts("Calling ls...");
	tat_ls();

	puts("That's me done");
	puts("I'm just going to hang now");

	while(1);

}

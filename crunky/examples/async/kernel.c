#include <lfb.h>
#include <yastdio.h>

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

	puts("That's me done");
	puts("I'm just going to hang now");

	while(1);

}

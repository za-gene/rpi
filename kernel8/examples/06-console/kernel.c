#include <lfb.h>

void kernel_main()
{
	lfb_init();

	// print a little triangle
	for(int i =0; i<8; i++) {
		for(int j = 0; j<i; j++) {
			fbputchar('*');
		}
		fbnewline();
	}

	fbputs("That's me done");
	fbputs("I'm just going to hang now");

	while(1);

}

#include <lfb.h>
void say(char *str)
{
    fbputs(str);
}

void kernel_main()
{
	lfb_init();
	say("keyobard test started");
	say("Let's try");


finis:
	while(1);
}

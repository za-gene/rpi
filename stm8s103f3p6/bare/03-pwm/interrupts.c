
void enable_interrupts()
{
	__asm
		rim
		__endasm;
}

void disable_interrupts()
{
	__asm
		sim
		__endasm;
}


/*
void foo()
{
	enable_interrupts();
	enable_interrupts();
	//disable_interrupts();
}
*/

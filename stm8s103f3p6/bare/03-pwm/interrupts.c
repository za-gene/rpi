void enable_interrupts_X()
{
	__asm
	rim
	__endasm;
}

void disable_interrupts_X()
{
	__asm
	sim
	__endasm;
}

int foo() 
{
	int c= 5;
	return c;
}

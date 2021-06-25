#define REG(addr) *(volatile u32*)(addr)

#define IO_BANK0_BASE 0x40014000

#define GPIO25_CTRL REG(IO_BANK0_BASE+0x0cc)


int main()
{

	volatile int i = 0;
	while(1) {
		i++;
	}

	return 0;
}

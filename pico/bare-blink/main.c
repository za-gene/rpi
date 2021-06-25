#define REG(addr) *(volatile u32*)(addr)

#define IO_BANK0_BASE 0x40014000

#define GPIO25_CTRL REG(IO_BANK0_BASE+0x0cc)


int main()
{

	while(1);
	return 0;
}

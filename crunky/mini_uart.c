#include <mini_uart.h>
#include <gpio.h>
#include <timers.h>

void uart_send ( char c )
{
	while(1) {
		if(get32(AUX_MU_LSR_REG)&0x20)
			break;
	}
	put32(AUX_MU_IO_REG,c);
}

char uart_recv ( void )
{
	while(1) {
		if(get32(AUX_MU_LSR_REG)&0x01)
			break;
	}
	return(get32(AUX_MU_IO_REG)&0xFF);
}

void uart_puts(char* str)
{
	for (int i = 0; str[i] != '\0'; i ++) {
		uart_send((char)str[i]);
	}
}

void delay_uart()
{
	delay_ms(1);
}

void uart_init ( int baud )
{
	//unsigned int selector;

    // the following two paragraphs are probably better handled by gpio_sel()
	//selector = get32(GPFSEL1);
	GPFSEL1 &= ~(7<<12);                   // clean gpio14
	GPFSEL1 |= 2<<12;                      // set alt5 for gpio14
	GPFSEL1 &= ~(7<<15);                   // clean gpio15
	GPFSEL1 |= 2<<15;                      // set alt5 for gpio15
	//put32(GPFSEL1,selector);

	//put32(GPPUD,0);
	GPPUD = 0;
	delay_uart();
	//put32(GPPUDCLK0,(1<<14)|(1<<15));
	GPPUDCLK0 = (1<<14)|(1<<15);
	delay_uart();
	//put32(GPPUDCLK0,0);
	GPPUDCLK0 = 0;

	put32(AUX_ENABLES,1);                   //Enable mini uart (this also enables access to it registers)
	put32(AUX_MU_CNTL_REG,0);               //Disable auto flow control and disable receiver and transmitter (for now)
	put32(AUX_MU_IER_REG,0);                //Disable receive and transmit interrupts
	put32(AUX_MU_LCR_REG,3);                //Enable 8 bit mode
	put32(AUX_MU_MCR_REG,0);                //Set RTS line to be always high
	//int baudrate_reg = 250000000/8/baud -1; // TODO Assumes 250Mhz system_clock_freq
	//put32(AUX_MU_BAUD_REG,270);             //Set baud rate to 115200
	//put32(AUX_MU_BAUD_REG,baudrate_reg);             //Set baud rate to 9600 mc
	int baudrate_reg;
	baudrate_reg = 270; // for baud 115200
	//baudrate_reg = 3254; // for baud 9600
	put32(AUX_MU_BAUD_REG, baudrate_reg); // baud 9600

	put32(AUX_MU_CNTL_REG,3);               //Finally, enable transmitter and receiver
}

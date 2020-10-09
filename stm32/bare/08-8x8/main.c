#include <dma.h>
#include <gpio.h>
#include <i2c.h>
#include <usart.h>

#define SID 0x70 // SID

void printi(u32 x)
{
	char buf[10];
	itoa(x, buf, 10);
	print(buf);
}



void init_i2c() // this seems to be correct
{
	//RCC->AB1ENR &= ~(RCC_APB1ENR_I2CEN);
	I2C1->CR1 = 0X00;
	I2C1->CR2 = 0X00;
	//RCC_APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
	RCC_APB2ENR |= RCC_APB2ENR_AFIOEN;
	RCC_APB1ENR |= RCC_APB1ENR_I2C1EN;

	//I2C1_->CR1 |= I2C_CR1_SWRST; // reset I2C for good measure. bad-t


	I2C1->CR2 |= 36; // FREQ OF APB1 BUS = 72MHZ/2 = 36MHZ

	/* TAKE THE PERIOS OF THE FREQ SET INTO CR2 EX: 1/10MHZ = 100NS
	   NOW PICK A FREQ YOU WANT THE I2C TO RUN AT (MAX 100KHZ IN STD
	   MODE) (400KHZ MAX IN FAST MODE)
	   THEN IF IN STD MODE GET THE PERIOD OF THAT FREQ
EX:1/100KHZ = 10US THEN DIVIDE BY 2 TO GET THE
HTIME BECAUSE THE DUTY CYCLE IS 50% SO HIGH TIME AND LOW TIME
IS THE SAME SO WE JUST DIVIDE PERIOD BY 2 TO GET
5US
THEN CCR REGISTER = 5US/100NS = 50
SOO CCR IS 50, THIS IS THE MAX TIME THE I2C WILL WAIT FOR A
RISE TIME AND NOT NECESSARILY THE RISE TIME THAT IT WILL
ACTUALLY
*/
	I2C1->CCR |= 180; // SPEED TO 100KHZ STD MODE MAX: I2C_PERIOD /2 * ABI = (1/100K/2)*36MHZ
	I2C1->TRISE |= 37; // 1000NS/(CR2 PERIOD=1/36MHZ) = TRISE +1
	I2C1->CR1 |= I2C_CR1_ACK; // ENABLE ACKS

	// stretch mode enabled by default

	// 7 bit addressing mode enabled by default



	//GPIOB->CRL = 0b11111111010010000100010001000100;
	gpio_mode(PB6, 0b1111); 
	gpio_mode(PB7, 0b1111);


	I2C1->CR1 |= I2C_CR1_PE; // ENABLE PERIPHERAL
}



//u8 i2c_buff[10];

void i2c_read_dma(u8 sid, u8* buffer, u32 len)
{
#define  CHAN DMA1->CHAN7
	//CHAN.CCR &= ~DMA_CCR_EN; // added mcarter. Seems to help clear DMA_ISR_TCIF7

	//u32 temp =0;
	//i2c_buff[0] = 0x00;
	//rcc->ahbenr |= RCC_AHBENR_DMA1EN;
	RCC_AHBENR |= RCC_AHBENR_DMA1EN;
	I2C1->CR2 |= I2C_CR2_DMAEN;

	I2C1->CR1 |= I2C_CR1_ACK; // ENABLE ACKS
	//DMA1_CHANNEL5->CMAR = (U32)I2C_BUFF;
	//CHAN.CMAR = (u32)i2c_buff;
	CHAN.CMAR = (u32)buffer;
	//DMA1_CHANNEL5->CPAR = (U32)&I2C2->DR;
	CHAN.CPAR = (u32)(&(I2C1->DR));
	//DMA1_CHANNEL5->CNDTR = len;
	CHAN.CNDTR = len;
	//DMA1_CHANNEL5->CCR |= DMA_CCR4_TCIE | DMA_CCR5_MINC | DMA_CCR5_EN;
	//CHAN.CCR = DMA_CCR_TCIE | DMA_CCR_MINC | DMA_CCR_EN;
	//CHAN.CCR |= DMA_CCR_DIR; // added by mcarter . not helps
	//DMA1->IFCR |= DMA_ISR_TCIF7; // added by mcarter. hinders-t
	CHAN.CCR = DMA_CCR_MINC | DMA_CCR_EN;

	I2C1->CR1 |= I2C_CR1_START;
	I2C1->CR2 |= I2C_CR2_LAST; // added mcarter help-f

	//ser.print(".");
	//ser.println(DMA1->ISR & DMA_ISR_TCIF7);

	while(!(I2C1->SR1 & I2C_SR1_SB));
	I2C1->DR = (sid << 1) + 1; // WRITE 0XA0; // SEND ADDR

	//ser.print("-");
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	u32 temp = I2C1->SR2;
	//ser.print("<"); //seems to reach here

	while(!(DMA1->ISR & DMA_ISR_TCIF7));
	//I2C1_->SR2; // added by mcarter. help-f

	//ser.print(">"); // no seems to reach here

	I2C1->CR1 |= I2C_CR1_STOP;
	CHAN.CCR &= ~DMA_CCR_EN; // added mcarter. Seems to help clear DMA_ISR_TCIF7
	DMA1->IFCR |= DMA_ISR_TCIF7; // added by mcarter. hinders-?
}

void i2c_read(u8 sid, u8* buffer, u32 len)
{
	//I2C1->CR1 |= I2C_CR1_ACK; // ENABLE ACKS
	I2C1->CR1 |= I2C_CR1_START;

	//ser.print(".");

	while (!(I2C1->SR1 & I2C_SR1_SB));
	I2C1->DR = (sid << 1) +1;
	while (!(I2C1->SR1 & I2C_SR1_ADDR));
	(void)I2C1->SR2;

	//print("1");
	//I2C1_->DR = (u32)i2c_buff; //ADDRESS TO RWITE TO
	//I2C1->DR = (u32)buffer; //ADDRESS TO RWITE TO
	for(int i = 0; i < len; i++) {
		while (!(I2C1->SR1 & I2C_SR1_RXNE));
		(void)I2C1->SR2;
		buffer[i] = I2C1->DR;
	}


	//print("2");
	// COULR DO THIS MULTIPLE TIMES TO SEND LOTS OF DATA
	//I2C2->DR = DATA;
	//while (!(I2C1->SR1 & I2C_SR1_RXNE));
	//(void)I2C1->SR2;

	//print("3-");
	I2C1->CR1 |= I2C_CR1_STOP;

}



// using dma is easier, and seems to be recommended
// I2C is on APB1. runs at half the clock speed. So, if 72MHz, it will run at 36MHz




void write_i2c(u8 sid, u8* buffer, u32 len)
{
	//U32 TEMP;
	I2C1->CR1 |= I2C_CR1_START; // GENERATE A START CONDITION

	while(!(I2C1->SR1 & I2C_SR1_SB));
	I2C1->DR = sid<<1;
	while(!(I2C1->SR1 & I2C_SR1_ADDR));
	I2C1->SR2;

	for(u32 i=0; i<len; i++) {
		I2C1->DR = buffer[i]; 
		while(!(I2C1->SR1 & I2C_SR1_TXE));
	}

	// COULR DO THIS MULTIPLE TIMES TO SEND LOTS OF DATA
	//I2C2->DR = DATA;
	//WHILE(!(I2C2->SR1 & I2C_SR1_TXE));


	I2C1->CR1 |= I2C_CR1_STOP;
}

#ifdef TUTORIAL

// taken from stm8
static void begin_i2c_write(uint8_t slave_id)
{
	I2C_CR2 |= I2C_CR2_ACK;  // set ACK
	I2C_CR2 |= I2C_CR2_START;  // send start sequence
	while (!(I2C_SR1 & I2C_SR1_SB));

	I2C_DR = slave_id << 1; // send the address and direction
	while (!(I2C_SR1 & I2C_SR1_ADDR));
	(void)I2C_SR3;   // read SR3 to clear ADDR event bit
}
#endif //TUTORIAL

static uint8_t  pattern[] = { 
	//0b11111111, // for testing purposes
	0b10000001,
	0b01000010,
	0b00100100,
	0b00010000,
	0b00001000,
	0b00100100,
	0b01000010,
	0b10000001
};

u8 i2c_buff[2];

void send_cmd(u8 cmd) {
	i2c_buff[0] = cmd;
	write_i2c(SID, i2c_buff, 1);
}

void write_row(uint8_t y, uint8_t xs) {
	// the row of xs beed to be quirkily transformed
	uint8_t bits = 0;
	for (int x = 0; x < 7; ++x) {
		bits <<= 1;
		bits |= (xs & 1);
		xs  >>= 1;
	}
	bits |= (xs << 7);

	// send x layout to device
	//send2(2 * y, bits);

	i2c_buff[0] = 2*y;
	i2c_buff[1] = bits;
	write_i2c(SID, i2c_buff, 2);

}

void setup() {

	//Wire.begin();
	init_serial();
	puts("stm32 i2c bare 8x8 here 3");

	//pu32("I2C1_->CR1", I2C1_->CR2);
	init_i2c();
	//pu32("I2C1_->CR1", I2C1_->CR2);
	send_cmd(0x20 | 1); // turn on oscillator
	send_cmd(0x81); // display on
	send_cmd(0xE0 | 0); // brightness to dimmest (but you should probably set it)

}

void loop() {
	for (int i = 0; i < 8; ++i) {
		u8 row = pattern[i];
		write_row(i, row);
		row = ((row & 0x01) ? 0x80 : 0x00) | (row >> 1); // rotate the row for fun
		pattern[i] = row;
	}
	for(int i=0; i< 60000; i++) nop(); // simple delay
	//delay(100);
}

int main()
{
	setup();
	while(1) loop();



	/*
	   while(1) {
	   static int i = 0;		
	   print("Begin reading attempt 1");
	   char str[10];
	   itoa(i++, str, 10);
	   puts(str);
	   i2c_read(SID, i2c_buff, 1);
	   itoa(i2c_buff[0], str, 10);
	   puts(str);
	   }
	   */
}

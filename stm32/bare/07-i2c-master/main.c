#include <dma.h>
#include <gpio.h>
#include <i2c.h>
#include <usart.h>

#define SID 4


#define RCC_BASE        0x40021000
#define RCC_APB1ENR   *(volatile uint32_t *)(RCC_BASE   + 0x1C) // page 148
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)

#define RCC_APB2ENR_AFIOEN (1<<0)







#define RCC_APB1ENR_I2C1EN (1<<21)

void init_i2c() // this seems to be correct
{
	//RCC->AB1ENR &= ~(RCC_APB1ENR_I2CEN);
	//I2C2->CR1 = 0X00;
	//I2C2->CR2 = 0X00;
	//RCC_APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
	RCC_APB2ENR |= RCC_APB2ENR_AFIOEN;
	RCC_APB1ENR |= RCC_APB1ENR_I2C1EN;



	I2C1_->CR2 |= 36; // FREQ OF APB1 BUS = 72MHZ/2 = 36MHZ

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
	I2C1_->CCR |= 180; // SPEED TO 100KHZ STD MODE MAX: I2C_PERIOD /2 * ABI = (1/100K/2)*36MHZ
	I2C1_->TRISE |= 37; // 1000NS/(CR2 PERIOD=1/36MHZ) = TRISE +1
	I2C1_->CR1 |= I2C_CR1_ACK; // ENABLE ACKS

	// stretch mode enabled by default

	// 7 bit addressing mode enabled by default



	//GPIOB->CRL = 0b11111111010010000100010001000100;
	gpio_mode(PB6, 0b1111); 
	gpio_mode(PB7, 0b1111);


	I2C1_->CR1 |= I2C_CR1_PE; // ENABLE PERIPHERAL
}



#define RCC_AHBENR   REG(RCC_BASE   + 0x14)
#define RCC_AHBENR_DMA1EN (1<<0)


u8 i2c_buff[10];

void i2c_read_dma(u8 sid, u8* buffer, u32 len)
{
	//u32 temp =0;
	//i2c_buff[0] = 0x00;
	//rcc->ahbenr |= RCC_AHBENR_DMA1EN;
	RCC_AHBENR |= RCC_AHBENR_DMA1EN;
	I2C1_->CR2 |= I2C_CR2_DMAEN;

	I2C1_->CR1 |= I2C_CR1_ACK; // ENABLE ACKS
#define  CHAN DMA1->CHAN7
	//DMA1_CHANNEL5->CMAR = (U32)I2C_BUFF;
	CHAN.CMAR = (u32)i2c_buff;
	//DMA1_CHANNEL5->CPAR = (U32)&I2C2->DR;
	CHAN.CPAR = (u32)(&(I2C1_->DR));
	//DMA1_CHANNEL5->CNDTR = len;
	CHAN.CNDTR = len;
	//DMA1_CHANNEL5->CCR |= DMA_CCR4_TCIE | DMA_CCR5_MINC | DMA_CCR5_EN;
	//CHAN.CCR = DMA_CCR_TCIE | DMA_CCR_MINC | DMA_CCR_EN;
	//CHAN.CCR |= DMA_CCR_DIR; // added by mcarter . not helps
	//DMA1->IFCR |= DMA_ISR_TCIF7; // added by mcarter. hinders-t
	CHAN.CCR = DMA_CCR_MINC | DMA_CCR_EN;

	I2C1_->CR1 |= I2C_CR1_START;
#define I2C_CR2_LAST (1<<12)
	I2C1_->CR2 |= I2C_CR2_LAST; // added mcarter help-f

	//ser.print(".");
	//ser.println(DMA1->ISR & DMA_ISR_TCIF7);

	while(!(I2C1_->SR1 & I2C_SR1_SB));
	I2C1_->DR = (sid << 1) + 1; // WRITE 0XA0; // SEND ADDR

	//ser.print("-");
	while (!(I2C1_->SR1 & I2C_SR1_ADDR));
	u32 temp = I2C1_->SR2;
	//ser.print("<"); //seems to reach here

	while(!(DMA1->ISR & DMA_ISR_TCIF7));
	//I2C1_->SR2; // added by mcarter. help-f

	//ser.print(">"); // no seems to reach here

	I2C1_->CR1 |= I2C_CR1_STOP;
	DMA1->IFCR |= DMA_ISR_TCIF7; // added by mcarter. hinders-?
	CHAN.CCR &= ~DMA_CCR_EN; // added mcarter. Seems to help clear DMA_ISR_TCIF7
}

void i2c_read(u8 sid, u8* buffer, u32 len)
{
	I2C1_->CR1 |= I2C_CR1_ACK; // ENABLE ACKS
	I2C1_->CR1 |= I2C_CR1_START;

	//ser.print(".");

	while (!(I2C1_->SR1 & I2C_SR1_SB));
	I2C1_->DR = (sid << 1); // WRITE 0XA0; // SEND ADDR
	while (!(I2C1_->SR1 & I2C_SR1_ADDR));
	(void)I2C1_->SR2;

	//ser.print("1");
	I2C1_->DR = (u32)i2c_buff; //ADDRESS TO RWITE TO
	while (!(I2C1_->SR1 & I2C_SR1_TXE));
	(void)I2C1_->SR2;

	//ser.print("2");
	// COULR DO THIS MULTIPLE TIMES TO SEND LOTS OF DATA
	//I2C2->DR = DATA;
	while (!(I2C1_->SR1 & I2C_SR1_RXNE));
	(void)I2C1_->SR2;

	//ser.print("3");
	I2C1_->CR1 |= I2C_CR1_STOP;

}



// using dma is easier, and seems to be recommended
// I2C is on APB1. runs at half the clock speed. So, if 72MHz, it will run at 36MHz


#ifdef TUTORIAL

VOID I2C_WRITE_SINGLE(U8 SID, I8 MEM_ADDR, U8 DATA)
{
	U32 TEMP;
	I2C2->CR1 |= I2C2_CR1_START; // GENERATE A START CONDITION

	WHILE(!(I2C2->SR1 & I2C_SR1_SB));
	I2C2->DR = SID;
	WHILE(!(I2C2->SR1 & I2C2_SR1_ADDR));
	TEMP = I2C2->SR2;

	I2C2->DR = MEM_ADDR; //ADDRESS TO RWITE TO
	WHILE(!(I2C2->SR1 & I2C2_SR1_TXE));

	// COULR DO THIS MULTIPLE TIMES TO SEND LOTS OF DATA
	I2C2->DR = DATA;
	WHILE(!(I2C2->SR1 & I2C_SR1_TXE));


	I2C2->CR1 |= I2C_CR1_STOP;
}

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













int main()
{
	init_serial();
	puts("i2c bare master here");

	//pu32("I2C1_->CR1", I2C1_->CR2);
	init_i2c();
	//pu32("I2C1_->CR1", I2C1_->CR2);

	while(1) {
		static int i = 0;		
		print("Begin reading attempt ");
		char str[10];
		itoa(i++, str, 10);
		puts(str);
		i2c_read_dma(SID, 0, 1);
		itoa(i2c_buff[0], str, 10);
		puts(str);
		for(int i=0; i< 600000; i++) nop(); // simple delay
	}
}

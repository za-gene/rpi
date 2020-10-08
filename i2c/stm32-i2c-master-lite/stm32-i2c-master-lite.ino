/*
   /home/pi/.arduino15/packages/stm32duino/hardware/STM32F1/2020.6.20/cores/maple/libmaple

*/

//#include <Wire.h>





#ifdef MCU_STM32F103C8
auto &ser = Serial1;
#else
auto &ser = Serial;
#endif

#define SID 4


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define __IO volatile

#define REG(x) *(volatile uint32_t *)(x)

typedef struct {
  __IO u32 CR1; //0x00
  __IO u32 CR2; // 0x04
  __IO u32 OAR1; //0x08
  __IO u32 OAR2; //0x0C
  __IO u32 DR; //0x10
  __IO u32 SR1; //0x14
  __IO u32 SR2; //0x18
  __IO u32 CCR; // 0x1C
  __IO u32 TRISE; // 0x20
} I2C_t;

#define I2C1_ ((I2C_t*) (0x40005400))
#define I2C2_ ((I2C_t*) (0x40005800))

//#define I2C_ I2C1_

#define I2C_CR1_PE (1<<0)


#define RCC_BASE        0x40021000
#define RCC_APB1ENR   *(volatile uint32_t *)(RCC_BASE   + 0x1C) // page 148
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)

#define RCC_APB2ENR_AFIOEN (1<<0)

typedef struct
{
  __IO uint32_t CRL; // 0x00
  __IO uint32_t CRH; // 0x04
  __IO uint32_t IDR; // 0x08
  __IO uint32_t ODR; // 0x0C
  __IO uint32_t BSRR; // 0x10
  __IO uint32_t BRR; // 0x14
  __IO uint32_t LCKR; //0x18
} GPIO_t;

#define GPIO_BASE 0x40010800
#define GPIOA ((GPIO_t*) (GPIO_BASE + 0x000))
#define GPIOB ((GPIO_t*) (GPIO_BASE + 0x400))



void pu32(char* str, u32 v) {
  ser.print(str);
  ser.print(v);
  ser.print(" 0b");
  for (int i = 0; i < 8; i++) {
    if (i) ser.print("'");
    for (int j = 0; j < 4; j++) {
      //u32 x = v & (0b10000000000000000000000000000000);
      u32 x = v & (1 << 31);
      if (x > 0) {
        ser.print(1);
      } else {
        ser.print(0);
      }

      v = (v << 1);
    }
  }
  ser.println("");
}

// stm32 i2c pt 1
// https://www.youtube.com/watch?v=TqUzQfMGlfI
// part 2
// https://www.youtube.com/watch?v=ZVeG25cMe58

/* send addr 1 means read, 0 meand write in LSB, but maybe only for AT32C02

*/





#define I2C_CR1_PE (1<<0)
#define RCC_APB1ENR_I2C1EN (1<<21)
#define I2C_CR1_ACK (1<<10)
#define I2C_CR1_START (1<<8)
#define I2C_CR1_STOP (1<<9)

#define I2C_SR1_TXE (1<<7)
#define I2C_SR1_RXNE (1<<6)
#define I2C_SR1_ADDR (1<<1)


void my_init_i2c() // this seems to be correct
{
  //RCC->AB1ENR &= ~(RCC_APB1ENR_I2CEN);
  //I2C2->CR1 = 0X00;
  //I2C2->CR2 = 0X00;
  RCC_APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
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
  I2C1_->CR1 |= I2C_CR1_ACK ; // ENABLE ACKS

  // stretch mode enabled by default

  // 7 bit addressing mode enabled by default



  GPIOB->CRL = 0b11111111010010000100010001000100;


  I2C1_->CR1 |= I2C_CR1_PE; // ENABLE PERIPHERAL
}



#define RCC_AHBENR   REG(RCC_BASE   + 0x14)
#define RCC_AHBENR_DMA1EN (1<<0)

#define I2C_CR2_DMAEN (1<<11)

typedef struct {
  __IO u32 CCR;
  __IO u32 CNDTR;
  __IO u32 CPAR;
  __IO u32 CMAR;
  __IO u32 RESERVED;
} DMA_CHAN_t;


typedef struct
{
  __IO u32 ISR;
  __IO u32 IFCR;
  DMA_CHAN_t CHAN1;
  DMA_CHAN_t CHAN2;
  DMA_CHAN_t CHAN3;
  DMA_CHAN_t CHAN4;
  DMA_CHAN_t CHAN5;
  DMA_CHAN_t CHAN6;
  DMA_CHAN_t CHAN7;
} DMA_t;

//#define GPIO_BASE 0x40010800
#define DMA1 ((DMA_t*) (0x40020000))
#define DMA2 ((DMA_t*) (0x40020400))

#define DMA_CCR_TCIE  (1<<1)
#define DMA_CCR_MINC (1<<7)
#define DMA_CCR_EN (1<<0)
#define DMA_CCR_DIR (1<<4)

#define DMA_ISR_TCIF5 (1<<17)
#define DMA_ISR_TCIF7 (1<<25)

#define I2C_SR1_SB (1<<0)

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

  ser.print(".");
  //ser.println(DMA1->ISR & DMA_ISR_TCIF7);

  while (!(I2C1_->SR1 & I2C_SR1_SB));
  I2C1_->DR = (sid << 1) + 1; // WRITE 0XA0; // SEND ADDR

  ser.print("-");
  while (!(I2C1_->SR1 & I2C_SR1_ADDR));
  u32 temp = I2C1_->SR2;
  ser.print("<"); //seems to reach here

  while (!(DMA1->ISR & DMA_ISR_TCIF7));
  //I2C1_->SR2; // added by mcarter. help-f

  ser.print(">"); // no seems to reach here

  I2C1_->CR1 |= I2C_CR1_STOP;
  DMA1->IFCR |= DMA_ISR_TCIF7; // added by mcarter. hinders-?
  CHAN.CCR &= ~DMA_CCR_EN; // added mcarter. Seems to help clear DMA_ISR_TCIF7
}

void i2c_read(u8 sid, u8* buffer, u32 len)
{
  I2C1_->CR1 |= I2C_CR1_ACK; // ENABLE ACKS

  I2C1_->CR1 |= I2C_CR1_START;

  ser.print(".");

  while (!(I2C1_->SR1 & I2C_SR1_SB));
  I2C1_->DR = (sid << 1); // WRITE 0XA0; // SEND ADDR
  while (!(I2C1_->SR1 & I2C_SR1_ADDR));
  (void)I2C1_->SR2;

  ser.print("1");
  I2C1_->DR = (u32)i2c_buff; //ADDRESS TO RWITE TO
  while (!(I2C1_->SR1 & I2C_SR1_TXE));
  (void)I2C1_->SR2;

  ser.print("2");
  // COULR DO THIS MULTIPLE TIMES TO SEND LOTS OF DATA
  //I2C2->DR = DATA;
  while (!(I2C1_->SR1 & I2C_SR1_RXNE));
  (void)I2C1_->SR2;

  ser.print("3");
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













void setup()
{
  ser.begin(115200); // start serial for output
  ser.println("i2c master here 5");

  //pu32("I2C1_->CR1", I2C1_->CR2);
#if 0
  Wire.begin(); // join i2c bus (address optional for master)
#else
  my_init_i2c();
#endif
  //pu32("I2C1_->CR1", I2C1_->CR2);

}

void loop()
{
  static int i = 0;
  //goto foo;
  ser.println("Begin reading attempt " + String(i++));
  i2c_read_dma(SID, 0, 1);
  ser.println(i2c_buff[0]); // print the character
  //ser.println(i2c_buff[1]); // print the character
  delay(1000);

  return;
  #if 0
foo:
  Wire.requestFrom(4, 1); // request 1 byte from slave device address 4

  while (Wire.available()) // slave may send less than requested
  {
    int i = Wire.read(); // receive a byte as character
    ser.println(i); // print the character
  }

  delay(1000);
  #endif
}

#include <Wire.h>





#ifdef MCU_STM32F103C8
auto &ser = Serial1;
#else
auto &ser = Serial;
#endif


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

#define I2C_ I2C1_

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
 *  
 */


void init_i2c()
{
  //RCC->AB1ENR &= ~(RCC_APB1ENR_I2CEN);
  //I2C2->CR1 = 0X00;
  //I2C2->CR2 = 0X00;
  RCC_APB2ENR |= RCC_APB2ENR_IOPBEN | RCC_APB2ENR_AFIOEN;
  RCC_APB1ENR |= RCC_APB1ENR_I2C2EN;

  I2C1_->CR2 |= 36; // FREQ OF APB1 BUS = 72MHZ/2 = 36MHZ

  /* TAKE THE PERIOS OF THE FREQ SET INTO CR2 EX: 1/10MHZ = 100NS
   *  NOW PICK A FREQ YOU WANT THE I2C TO RUN AT (MAX 100KHZ IN STD
   *  MODE) (400KHZ MAX IN FAST MODE)
   *  THEN IF IN STD MODE GET THE PERIOD OF THAT FREQ
   *  EX:1/100KHZ = 10US THEN DIVIDE BY 2 TO GET THE
   *  HTIME BECAUSE THE DUTY CYCLE IS 50% SO HIGH TIME AND LOW TIME
   *  IS THE SAME SO WE JUST DIVIDE PERIOD BY 2 TO GET
   *  5US
   *  THEN CCR REGISTER = 5US/100NS = 50
   *  SOO CCR IS 50, THIS IS THE MAX TIME THE I2C WILL WAIT FOR A 
   *  RISE TIME AND NOT NECESSARILY THE RISE TIME THAT IT WILL
   *  ACTUALLY
   */
   I2C1_->CCR |= 180; // SPEED TO 100KHZ STD MODE MAX: I2C_PERIOD /2 * ABI = (1/100K/2)*36MHZ
   I2C1_->TRISE |= 37; // 1000NS/(CR2 PERIOD=1/36MHZ) = TRISE +1
   I2C1_->CR1 |= I2C_CR1_ACK ; // ENABLE ACKS

   // stretch mode enabled by default

   // 7 bit addressing mode enabled by default

// PAGE 168
   // configu gpio for internal pullup alt function open drain OUTPUT
   //GPIOB->CRH |= GPIO_CRH_CNF10 | GPIO_CRH_MODE_0
   //| BPIO_CRH_CNF11 | BPIO_CRH_MODE11;
   //#define GPIOA ((GPIO_t*) (GPIO_BASE + 0x000))
   GPIOB->CRL = 0b11111111010010000100010001000100;


   I2C1_->CR1 |= I2C_CR1_PE; // ENABLE PERIPHERAL
}



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

void i2c_read(u8 sid, u8 len)
{
  u32 temp =0;
  i2c_buff[0] = 0x00;
  rcc->ahbenr |= RCC_AHBENR_DMA1EN;
  I2C2->CR2 |= I2C2_CR2_DMAEN;
  I2C2->CR1 |= I2C_CR1_ACK; // ENABLE ACKS
  DMA1_CHANNEL5->CMAR = (U32)I2C_BUFF;
  DMA1_CHANNEL5->CPAR =(U32)&I2C2->DR;
  DMA1_CHANNEL5->CNDTR = len;
  DMA1_CHANNEL5->CCR |= DMA_CCR4_TCIE | DMA_CCR5_MINC | DMA_CCR5_EN;

  I2C2->CR1 |= I2C_CR1_START;

  WHILE(!(I2C2->SR1 & I2C_SR1_SB));
  I2C2->DR = SID+1; // WRITE 0XA0; // SEND ADDR

  WHILE(!(I2C2->SR1 & I2C_SR1_ADDR));
  TEMP = I2C2->SR2;

  WHILE((DMA1->ISR & DMA_ISR_TCIF5)==0);

  I2C2->CR1 |= I2C_CR1_STOP;
}




// using dma is easier, and seems to be recommended
// I2C is on APB1. runs at half the clock speed. So, if 72MHz, it will run at 36MHz

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
  ser.println("i2c master here");
    
  pu32("CR", GPIOB->CRL); // FREQ OF APB1 BUS = 72MHZ/2 = 36MHZ
  //Wire.begin(); // join i2c bus (address optional for master)
  init_i2c();
  pu32("CR", GPIOB->CRL); // FREQ OF APB1 BUS = 72MHZ/2 = 36MHZ

}

void loop()
{
  //return;
  Wire.requestFrom(4, 1); // request 1 byte from slave device address 4

  while (Wire.available()) // slave may send less than requested
  {
    int i = Wire.read(); // receive a byte as character
    ser.println(i); // print the character
  }

  delay(1000);
}

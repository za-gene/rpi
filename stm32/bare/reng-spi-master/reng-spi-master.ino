// files:
// /home/pi/.arduino15/packages/stm32duino/hardware/STM32F1/2020.6.20/libraries/SPI/src/SPI.cpp

/*
   a minimal example that works

*/

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#define __IO volatile

#define REG(x) *(volatile uint32_t *)(x)

// section 25.5 page 742
typedef struct {
  __IO u32 CR1;     // 0x00
  __IO u32 CR2;     // 0x04
  __IO u32 SR;      // 0x08
  __IO u32 DR;      // 0x0C
  __IO u32 CRCPR;   // 0x10
  __IO u32 RXCRCR;  // 0x14
  __IO u32 TXCRCR;  // 0x18
  __IO u32 I2SCFGR; // 0x1C
  __IO u32 I2SPR;   // 0x20
} SPI_t;

//#define SPI1x ((SPI_t*) 0x40013000)
#define SPI1 ((SPI_t*) 0x40013000)
//#define SPI3 ((SPI_t*) 0x40003C00)
//#define SPI2 ((SPI_t*) 0x40003800)

#define RCC_BASE        0x40021000
#define RCC_APB1ENR   *(volatile uint32_t *)(RCC_BASE   + 0x1C) // page 148
#define RCC_APB2ENR   *(volatile uint32_t *)(RCC_BASE   + 0x18)


#define RCC_APB2ENR_SPI1EN (1<<12)

#define SPI_CR1_SPE (1<<6)
#define SPI_CR1_SSM (1<<9)
#define SPI_CR1_SSI (1<<8)
#define SPI_CR1_MSTR (1<<2)
#define SPI_CR1_BIDIMODE (1<<15)
#define SPI_CR1_BIDIOE (1<<14)

#define SPI_CR2_SSOE (1<<2)

#define SPI_SR_RXNE (1<<0)
#define SPI_SR_TXE (1<<1)
#define SPI_SR_BSY (1<<7)

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

auto& ser = Serial1;

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
//#include <SPI.h>

#define SS PA4
static const int spiClk = 250000; // 250kHz

void  init_spi() {

  //SPI1x->CR1 = 852;
  RCC_APB2ENR |= RCC_APB2ENR_SPI1EN;
  SPI1->CR1 |= SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR ;

  // set the baud. If you don't get it right, you'll get rubbish results
  SPI1->CR1 &= ~(0b111<<3); // clear the baud rate
  u32 baud = 0b010 << 3;
  SPI1->CR1 |= baud;

  SPI1->CR1 |= SPI_CR1_SPE; // enable
  //SPI1->CR1 = 852;
    
  //RCC_APB2ENR |= 1 << 12; // SPI1EN

  // tricky to accomplish in Arduino IDE: this sets PA7 and PA5 is alternate output (pattern 0b1011)
  GPIOA->CRL = 0b10110100101100110100010001000100;
  //pinMode(PA5, OUTPUT);
  //pinMode(PA6,INPUT);
  //pinMode(PA7, OUTPUT);
}

void setup() {
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  ser.begin(115200);
  ser.println("Hello from stm32 spi master 3");

  //pu32("CRL", GPIOA->CRL);
  init_spi();
  pu32("CRL", GPIOA->CRL);
  //pu32("CR1", SPI1->CR1);
}

u8 spi_transfer(u8 data)
{
  SPI1->DR; // read any previous data
  SPI1->DR = data; // write the data

  // wait until transmission complete
  while (!(SPI1->SR & SPI_SR_TXE));
  while (SPI1->SR & SPI_SR_BSY);

  return (u8)SPI1->DR; // return the result
}




void loop() {
  byte data = 0b01010101; // junk data to illustrate usage

  digitalWrite(SS, LOW); //pull SS slow to prep other end for transfer
  int res = spi_transfer(data);
  digitalWrite(SS, HIGH); //pull ss high to signify end of data transfer
  ser.print("stm32 master: slave returned: ");  ser.println(res);
  delay(1000);
}

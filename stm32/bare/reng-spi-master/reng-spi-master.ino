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

#define SPI1x ((SPI_t*) 0x40013000)
#define SPI1 SPI1x
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
#if 0
  SPI.begin();
  SPI.setClockDivider(SPI_CLOCK_DIV8);//divide the clock by 8
  //SPI.setClockDivider(SPI_CLOCK_DIV16);//divide the clock by 8
#else
  //pinMode(PA5, OUTPUT);
  //pinMode(PA6,INPUT);
  //pinMode(PA7, OUTPUT);
  SPI1x->CR1 = 852;
  RCC_APB2ENR |= 1 << 12; // SPI1EN
  GPIOA->CRL = 0b10110100101100110100010001000100;
#endif
}

void setup() {
  pinMode(SS, OUTPUT);
  digitalWrite(SS, HIGH);

  ser.begin(115200);
  ser.println("Hello from stm32 spi master 3");

  pu32("CRL", GPIOA->CRL);
  init_spi();
  pu32("CRL", GPIOA->CRL);
}

u8 spi_rx_reg() { // guess - seems OK
  return SPI1x->DR;
}

void spi_tx_reg(u8 data) { // guess - seems OK
  SPI1x->DR = data;
}

void waitSpiTxEnd() { // seems to corespond with official
  while (!(SPI1x->SR & SPI_SR_TXE));
  while (SPI1x->SR & SPI_SR_BSY);
}

/*
  static inline void waitSpiTxEnd(spi_dev *spi_d)
  {
    while (spi_is_tx_empty(spi_d) == 0); // wait until TXE=1
    while (spi_is_busy(spi_d) != 0); // wait until BSY=0
  }
*/

//This actually works!!!
int spi_transfer_1(u8 data) {
#if 0
  return SPI.transfer(data);
#else
  spi_rx_reg(); // read any previous data
  spi_tx_reg(data); // write the data item in SP_DR
  waitSpiTxEnd();
  return (u8)spi_rx_reg();
#endif
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

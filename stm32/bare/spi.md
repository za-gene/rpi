# spi

##  uni/bi-directional and sim/half/du-plex modes explained

Uni-directional: data is transmitted by either slave or master, but not both
Bi-directional: data is transmitted by both


Full duplex: two data lines (MOSI and MISO). Master and slave transmits simultaneously
Half-duplex: one data line, devices take turns receiving and transmitting
simplex: one data line, data only going in one direction; be it master to slave, or slave to master

See also: [video](https://www.youtube.com/watch?v=HgvID9QMg_A)



## SSM: slave select management


SS (Slave Select) aka CS (Chip Select) is referred to in the datasheet as NSS. It is described in section 26.3.5. Hardware or software SS management can se set using the SSM bit in the SPI_CR1 register.

Setting SSM=1  sets software managment. You must manually set SS low and high again.

Setting SSM=0 sets hardware managment. In this mode SS is set low/high automaticially by the IC. 

Hardware management might be more convenient, but won't allow for controlling multiple slaves. The STM32 also has facilities for managing bus contention in multi-master setups. I will not discuss that here.

See also: [video](https://www.youtube.com/watch?v=HjMF4w8QTn8)

## registers

DFF = 0 means an 8-bit data frame, Set to 1 for 16-bit data frame. This bit should be written only when SPI is disabled (SPE=0).

PA4 SPI1\_NSS, PA5 SPI1\_SCK, PA6 SPI1\_MISO, PA7 SPI\_MOSI
void setupSPI()
{
// turn on the clocks
RCC_IOPENR |= RCC_IOPENR_GPIOAEN; // enable port A, where our SPI is
RCC_APB2ENR |= RCC_APB2ENR_SPI1EN; // enable SPI1

// set up the alternate function - my code will be different
gpioa->moder &= ~(gpio_moder_mode5 | gpio_moder_mode7) // pa5 and pa7 clear
gpioa->moder |= gpioa_mode5_1 | gpioa_mode7_1 ; set to alternate function
//spi is alternate function 0 in GPIOx_AFRL, which is already set

// configure spi
SPI_CR1 |= SPI_CR1_BIDIMODE // 1-line bi-directional
    | BIDIOE // transmit-only
// check on RXONLY - shouldn't be applicable
    | SPI_CR1_SSM // we'll manage the SS pins ourselves
    | SPI_CR1_MSTR // set master mode
    ;

// set baud rate BR register

SPI1->CR2 |= spi_cr2_ssoe; 

SPI_CR1_SPE; // enable SPI
}



void spi_send(u8 data)
{
spi1->dr = data;
while(!(spi1->sr & spi_sr_txe)); // wait until the data is transmitted
}

See also: [video](https://www.youtube.com/watch?v=0kFb6_f9u8w)

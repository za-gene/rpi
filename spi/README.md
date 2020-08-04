# SPI

## Pinouts

```
NAME	  WROOM		Pi3	STM32	NANO	DUP	DUP
	---------
	VSPI HSPI
CLK	 D18  D14	23	PA5	D13	WHITE	GREEN
MISO	 D19  D12	21	PA6	D12	GREY	YELLOW
MOSI	 D23  D13	19	PA7	D11	PURPLE	ORANGE
CS	 D05  D15	24	PA4	D10	BLACK	BLUE

WROOM	- GPIO PINOUT ON ESP32-WROOM (VSPI and HSPI)
Pi3	- PINOUTS ON PI3 PHYSICAL BOARD PINS
STM32	- BLUE PILL (Default as used by stm-spi-slave example)
NANO	- NANO or UNO
DUP	- SUGGESTED DUPONT COLOURS
```

## In this directory

* [rpi-master-spi.py](rpi-master-spi.py) - example which has been proven to work

## Unclassified

* MODE 0: data is sampled at the leading rising edge


## Links to other sites

* [Basics of SPI](https://www.electronicshub.org/basics-serial-peripheral-interface-spi/)

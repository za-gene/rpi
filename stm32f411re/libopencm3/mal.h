#pragma once
/**  @defgroup mal Mal

@brief Declarations for Mark's Abstraction Layer 

mal.h to sort out by mc

*/

/*
 * Are we there yet?
 */


#include <stdint.h>


void mal_max7219_init(void);
void mal_max7219_show_count(int count);
void mal_max7219_tfr(uint8_t address, uint8_t value);

void mal_spi_init_std(void);

typedef struct {
	uint32_t port;
	int num;
	//rcc_periph_clken rcc;
	//rcc_periph_clken rcc;
	int rcc;
} pin_t;

extern pin_t pb12, pc13, pc14;

#define PB12 &pb12
#define PC13 &pc13
#define PC14 &pc14

void pin_toggle(pin_t* pin);
void pin_out(pin_t* pin);
void pin_high(pin_t* pin);
void pin_low(pin_t* pin);


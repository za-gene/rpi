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

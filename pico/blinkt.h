#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void blinkt_init(uint8_t dataPin, uint8_t clockPin);
void blinkt_show();
void blinkt_set_pixel_colour(uint8_t pos , uint8_t r, uint8_t g, uint8_t b);

#ifdef __cplusplus
}
#endif


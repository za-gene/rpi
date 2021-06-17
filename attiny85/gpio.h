#pragma once

#include <stdint.h>

#define LOW	0
#define HIGH	1

#define OUTPUT 		0
#define INPUT 		1
#define INPUT_PULLUP	2

void digitalWrite(uint8_t pin, int value);
void pinMode(uint8_t pin, uint8_t mode);
int digitalRead(uint8_t pin);

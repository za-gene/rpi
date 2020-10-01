#include <stdio.h>
#include <stdbool.h>

#include <gpio.h>


// section 10.1.2 Vector table

//#define NVIC_TIM4 *(volatile uint32_t *)(0x000000B8)

#include <SPI.h>

#ifdef ARDUINO_ARCH_ESP32 // ESP32-WROOM
// Connect as per parent directory
constexpr int rs_pin = 4;
constexpr int cs_pin = 5;
#endif

#ifdef MCU_STM32F103C8 // blue pill
constexpr int rs_pin = PB0;
constexpr int cs_pin = PA4;
#endif

#if 0 // Assume Nano
constexpr int rs_pin = 12;
constexpr int cs_pin = 10;
#endif

/* rs_val = LOW to send command, HIGH to send ASCII char 
*/
void sendByte(int rs_val, int val) {
	digitalWrite(rs_pin, rs_val);
	digitalWrite(cs_pin, LOW);
	SPI.transfer(val);
	digitalWrite(cs_pin, HIGH);  
	delay(60);
}

void main() {
	pinMode(cs_pin, OUTPUT);
	digitalWrite(cs_pin, HIGH);
	pinMode(rs_pin, OUTPUT);
	SPI.begin();
	SPI.setBitOrder(MSBFIRST);
	Serial.begin(115200);

	constexpr auto contrast = 0x70  | 0b1000; // from 0x7C
	constexpr auto display = 0b1111; // ori 0x0F
	uint8_t cmds[] = {0x39, 0x1D, 0x50, 0x6C, contrast , 0x38, display, 0x01, 0x06};
	for(int i=0; i<sizeof(cmds); ++i) sendByte(LOW, cmds[i]);
	Serial.println(sizeof(cmds));

	// now send some intersting output
	uint8_t msg[] = {'S', 'T', 'M', '3', '2'};
	for(int i=0; i<sizeof(msg); ++i) sendByte(HIGH, msg[i]);
	Serial.println(sizeof(msg));

	while(1);
}




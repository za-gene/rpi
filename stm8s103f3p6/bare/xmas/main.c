#include <stm8.h>
#include <gpio.h>
#include <millis.h>

typedef signed int sint;
typedef unsigned int uint;

sint pins[] = {PA1, PA2, PA3, PC3, PC4, PC5, PC6, PC7, PD1, PD2, PD3, PD4, PD5, PD6};


int rand()
{
	static int m_z = 42, m_w = 42;
	m_z = 36969 * (m_z & 65535) + (m_z >> 16);
	m_w = 18000 * (m_w & 65535) + (m_w >> 16);
	return (m_z << 16) + m_w;
}

void main()
{
	init_millis();
	for(int i = 0; i<sizeof(pins); i++) {
		gpio_mode_out(pins[i]);
		gpio_write(pins[i], 1);
		//int hi = random(3);
		//digitalWrite(pins[i], hi);
	}

	while (1)
	{
		for(int i = 0; i<sizeof(pins); i++) {
			int hi = rand() % 2;
			hi = 1;
			//u8 high = gpio_read(BUTTON);
			gpio_write(pins[i], hi);
		}
		delay_millis(1000);
	}
}


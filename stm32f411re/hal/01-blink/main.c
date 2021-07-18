//#define STM32F412Cx // /home/pi/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include
#include <stm32f4xx_hal.h>
//#include <stm32f4xx_hal_gpio.h>

int main()
{
	//HAL_Init();

	while(1) {
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);
		HAL_Delay(900);
	}
}

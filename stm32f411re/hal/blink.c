#include "stm32f4xx_hal.h"

void SysTick_Handler(void)
{
	HAL_IncTick();
}

void setup_gpio_pc13_as_output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};	
	__HAL_RCC_GPIOC_CLK_ENABLE(); /* GPIO Ports Clock Enable */
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET); 	/*Configure GPIO pin Output Level */

	/*Configure GPIO pin : PC13 */
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

int main()
{
	HAL_Init();	      // required
	setup_gpio_pc13_as_output();

	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_SET);

	while (1)
	{
		HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
		HAL_Delay(100*1);
	}
}
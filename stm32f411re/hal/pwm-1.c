#include "stm32f4xx_hal.h"
TIM_HandleTypeDef htim1;

static void MX_TIM1_Init(uint32_t freq, uint32_t duty_cycle_pc)
{
	htim1.Instance = TIM1;
	htim1.Init.Prescaler = HAL_RCC_GetSysClockFreq() / 1000000 - 1;
	htim1.Init.Period = 1000000 / freq - 1;
	HAL_TIM_Base_Init(&htim1);
	HAL_TIM_PWM_Init(&htim1);

	TIM_OC_InitTypeDef sConfigOC = {0};
	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = 1000000 / freq * duty_cycle_pc/100 - 1;
	HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4);

	/**TIM1 GPIO Configuration: PA11     ------> TIM1_CH4 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = GPIO_PIN_11;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

int main(void)
{
	HAL_Init();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_TIM1_CLK_ENABLE();
	HAL_NVIC_EnableIRQ(TIM1_CC_IRQn);

	MX_TIM1_Init(500, 25);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	while (1)
		;
}

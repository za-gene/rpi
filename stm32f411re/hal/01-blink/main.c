
int main()
{
	//HAL_Init();

	while(1) {
		HAL_GPIO_WritePin(GPIOC, GPIO13, GPIO_PIN_SET);
		HAL_Delay(100);
		HAL_GPIO_WritePin(GPIOC, GPIO13, GPIO_PIN_RESET);
		HAL_Delay(900);
	}
}

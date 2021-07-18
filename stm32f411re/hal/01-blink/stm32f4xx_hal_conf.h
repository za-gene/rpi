#pragma once
/* May not be able to avoid creating this file as other includes look for it */

#define STM32F412Cx // /home/pi/STM32Cube/Repository/STM32Cube_FW_F4_V1.25.2/Drivers/CMSIS/Device/ST/STM32F4xx/Include
#define  TICK_INT_PRIORITY            (0x0FU)
//#define HAL_MODULE_ENABLED
//#define HAL_GPIO_MODULE_ENABLED
//#define HAL_RCC_MODULE_ENABLED
//#define HAL_CORTEX_MODULE_ENABLED
#include <stm32f4xx_hal_rcc.h>
#include <stm32f4xx_hal_cortex.h>
#include <stm32f4xx_hal_gpio.h>

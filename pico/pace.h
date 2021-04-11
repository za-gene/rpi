#pragma once

#include "hardware/irq.h"

#ifdef __cplusplus
extern "C" {
#endif


//#include <stdint.h>

/** NB clock divider must be in range 1.f <= value < 256.f
 * Example usage in beatbyte project
*/

float pace_pwm_divider(int freq, int top);


/** 
 * Returns: 0 for OK. 1 for divider error. (You'll need to choose a difference freq/top combo
 * Example usage in beatbyte Project
 */
int pace_config_pwm_irq(unsigned int* slice_num, unsigned int gpio, int freq, int top, irq_handler_t pwm_irq_wrap_handler);


#ifdef __cplusplus
}
#endif



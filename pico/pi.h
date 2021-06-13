#pragma once

#include "hardware/irq.h"

#ifdef __cplusplus
extern "C" {
#endif



void pi_alarm_init(uint alarm_num, irq_handler_t callback, uint64_t delay_us);
void pi_alarm_rearm(int alarm_num, uint64_t delay_us);


#ifdef __cplusplus
}
#endif

#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "sam.h"


uint8_t timer_pwm_init();

uint8_t timer_set_duty_cycle(double percentage); 

uint32_t timer_read_cv();


#endif
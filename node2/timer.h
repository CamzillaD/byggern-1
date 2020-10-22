#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "sam.h"


uint8_t timer_init();

uint8_t timer_enable();

uint8_t timer_disable();

uint32_t timer_read_cv();


#endif
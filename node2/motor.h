#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include "sam.h"




void motor_dac_init();

void motor_dac_set_speed(int16_t data);

void motor_init();

uint32_t motor_turnon();

uint32_t motor_turnoff();

void delay(uint32_t ms);

uint16_t motor_read_encoder();

void motor_reset_encoder();


#endif
#ifndef MOTOR_H
#define MOTOR_H

#include <stdint.h>
#include "sam.h"
#include "pid_regulator.h"

void motor_dac_init();

void motor_dac_set_speed(int16_t data);

void motor_init();

void motor_turnon();

void motor_turnoff();

void delay(uint32_t ms);

void motor_reset_encoder();

void motor_encoder_init();

void motor_go_to_position(uint8_t pos);

uint16_t motor_read_encoder();


#endif
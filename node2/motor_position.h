#ifndef MOTOR_POSITION_H
#define MOTOR_POSITION_H
#include <stdint.h>

void motor_position_init();

void motor_position_set_reference(uint8_t position);

void motor_position_tracking_enable();

void motor_position_tracking_disable();

#endif
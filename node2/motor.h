#ifndef MOTOR_H
#define MOTOR_H
#include <stdint.h>

void motor_init();

void motor_command_speed(uint8_t speed);

void motor_command_position(uint8_t position);

uint16_t motor_encoder_read();

void motor_delay(uint32_t ms_approx);

#endif

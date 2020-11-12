#ifndef MOTOR_H
#define MOTOR_H
#include <stdint.h>
#include "pid_regulator.h"

void motor_init(Pid motor_position_pid);

void motor_command_speed(uint8_t speed);

void motor_command_position(uint8_t position);

uint16_t motor_encoder_read();


#endif

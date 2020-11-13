#ifndef SERVO_H
#define SERVO_H
#include <stdint.h>

void servo_init();

void servo_command_gimbal(uint8_t gimbal);

#endif
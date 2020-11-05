#ifndef SERVO_H
#define SERVO_H
#include <stdint.h>

#define SERVO_VOLTAGE_MIN   0.25
#define SERVO_VOLTAGE_MAX   0.5

//will use our controller output (pådrag) to calculate the correct duty cycle time

uint32_t servo_convert(double voltage);

#endif
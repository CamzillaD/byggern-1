#ifndef MOTOR_H
#define MOTOR_H
#include <stdint.h>

void motor_init();

void motor_command_speed(int16_t speed);

/**
 * @brief Command motor position.
 *
 * Range: [-100, 100], where -100 is all the way to
 * the left; 100 is all the way to the right, and 0
 * is the neutral position.
 *
 * @param position Commanded position.
 */
void motor_command_position(int16_t position);

uint16_t motor_encoder_read();

#endif

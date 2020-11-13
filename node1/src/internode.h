#ifndef INTERNODE_H
#define INTERNODE_H
#include <stdint.h>

void internode_reset();

void internode_speed(uint8_t speed);

void internode_position(uint8_t pos);

void internode_servo(uint8_t servo);

void internode_solenoid(uint8_t active);

uint8_t internode_end_game();

#endif
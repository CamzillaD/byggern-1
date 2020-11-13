#ifndef ENCODER_H
#define ENCODER_H
#include <stdint.h>

void encoder_init();

void encoder_reset();

uint16_t encoder_read();

#endif
#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void display_init();

void display_clear();

void display_print(uint8_t line, const char * string, uint8_t arrow);

void display_print_heading(const char * string);

void display_print_number(uint8_t line, uint16_t number);

#endif

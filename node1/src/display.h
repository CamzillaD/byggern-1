#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void display_init();

void display_clear();

void display_print(uint8_t line, const char * string);

void display_print_heading(const char * string);

#endif

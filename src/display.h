#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

void display_init();

void display_reset();

void display_push_ram_to_oled();

void display_home();

void display_go_to_line(uint8_t line);

void display_go_to_column(uint8_t column);

void display_clear_line(uint8_t line);

void display_pos(uint8_t row, uint8_t column);

void display_print(uint8_t* chr);

void display_clear();

#endif
#include "display.h"
#include <stdint.h>

#define DISPLAY_C_MEM ((volatile uint8_t *)0x1000)
#define DISPLAY_D_MEM ((volatile uint8_t *)0x1200)

void display_init(){
    *DISPLAY_C_MEM = 0x00;
}
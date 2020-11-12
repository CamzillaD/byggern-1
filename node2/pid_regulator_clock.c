#include "pid_regulator_clock.h"
#include <stdint.h>

#define SYSTICK_CTRL (*((uint32_t*)(0xe000e010)))
#define SYSTICK_LOAD (*((uint32_t*)(0xe000e014)))

void pid_regulator_clock_enable(){
    /* SysTick every 0.1 ms */
    SYSTICK_LOAD = 0x20d0;

    /* Enable SysTick, Interrupt, and Master clock */
    SYSTICK_CTRL = 0x07;
}

void pid_regulator_clock_disable(){
    /* Disable interrupt */
    SYSTICK_CTRL = 0x05;
}

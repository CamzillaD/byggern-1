#include "pid_regulator_clock.h"
#include "pid_regulator.h"
#include <stdint.h>

#define SYSTICK_CTRL (*((uint32_t*)(0xe000e010)))
#define SYSTICK_LOAD (*((uint32_t*)(0xe000e014)))

static Pid * mp_pid;
static double * mp_set_point;
static double * mp_sample;

void SysTick_Handler(){
    pid_step(mp_pid, *mp_set_point, *mp_sample);
}

void pid_regulator_clock_init(
    Pid * p_pid,
    double * p_set_point,
    double * p_sample
){
    mp_pid = p_pid;
    mp_set_point = p_set_point;
    mp_sample = p_sample;
}

void pid_regulator_clock_enable(){
    /* SysTick every 30 ms */
    SYSTICK_LOAD = 0x2673c0;

    /* Enable SysTick, Interrupt, and Master clock */
    SYSTICK_CTRL = 0x07;
}

void pid_regulator_clock_disable(){
    /* Disable interrupt */
    SYSTICK_CTRL = 0x05;
}

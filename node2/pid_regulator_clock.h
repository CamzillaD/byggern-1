#ifndef PID_REGULATOR_CLOCK_H
#define PID_REGULATOR_CLOCK_H
#include "pid_regulator.h"

void pid_regulator_clock_init(
    Pid * p_pid,
    double * p_set_point,
    double * p_sample
);

void pid_regulator_clock_enable();

void pid_regulator_clock_disable();

#endif

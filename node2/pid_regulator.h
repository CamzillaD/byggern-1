#ifndef PID_REGULATOR_H
#define PID_REGULATOR_H

typedef struct {
    /* Gains */
    float k_p;
    float k_i;
    float k_d;

    /* Sample time */
    float t;

    /* Derivative low-pass filter time constart */
    float tau;

    /* Integrator limits */
    float integral_min;
    float integral_max;

    /* Output clamping */
    float u_min;
    float u_max;

    /* Output bias */
    float u_bias;

    /* Previous steps */
    float integrator;
    float differentiator;
    float prev_sample;
    float prev_error;
} Pid;

Pid pid_regulator_new();

float pid_regulator_step(Pid * p_pid, float set_point, float sample);

#endif

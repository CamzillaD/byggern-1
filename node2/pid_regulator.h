#ifndef PID_REGULATOR_H
#define PID_REGULATOR_H

typedef struct {
    /* Gains */
    double k_p;
    double k_i;
    double k_d;

    /* Sample time */
    double t;

    /* Derivative low-pass filter time constart */
    double tau;

    /* Integrator limits */
    double integral_min;
    double integral_max;

    /* Output clamping */
    double u_min;
    double u_max;

    /* Previous steps */
    double integrator;
    double differentiator;
    double prev_sample;
    double prev_error;
} Pid;

Pid pid_new();

double pid_step(Pid * p_pid, double set_point, double sample);

#endif

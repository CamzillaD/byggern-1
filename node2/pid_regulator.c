#include "pid_regulator.h"

Pid pid_new(){
    Pid pid;

    pid.integrator = 0.0;
    pid.differentiator = 0.0;

    pid.prev_sample = 0.0;
    pid.prev_error = 0.0;

    return pid;
}

double pid_get_u(Pid * p_pid, double set_point, double sample){
    double error = set_point - sample;

    /* Proportional */
    double p_term = p_pid->k_p * error;

    /* Integral */
    p_pid->integrator = p_pid->integrator
                      + 0.5 * p_pid->k_i
                      * p_pid->t
                      * (error + p_pid->prev_error);

    if(p_pid->integrator > p_pid->integral_max){
        p_pid->integrator = p_pid->integral_max;
    }
    if(p_pid->integrator < p_pid->integral_min){
        p_pid->integrator = p_pid->integral_min;
    }

    /* Derivative */
    double sample_diff = sample - p_pid->prev_sample;
    p_pid->differentiator = - (2.0 * p_pid->k_d * sample_diff
                            + (2.0 * p_pid->tau - p_pid->t)
                            * p_pid->differentiator)
                            / (2.0 * p_pid->tau + p_pid->t);

    /* Output clamp */
    double u = p_term + p_pid->integrator + p_pid->differentiator;

    if(u > p_pid->u_max){
        u = p_pid->u_max;
    }
    if(u < p_pid->u_min){
        u = p_pid->u_min;
    }

    p_pid->prev_error = error;
    p_pid->prev_sample = sample;

    return u;
}

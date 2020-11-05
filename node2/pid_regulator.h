#ifndef PID_REGULATOR_H
#define PID_REGULATOR_H

typedef struct {
    double k_p;
    double k_i;
    double k_d;
    double sample_time;
} PidConfig;

void pid_regulator_init(PidConfig pid_config);

double pid_regulator_get_u(double set_point, double sample);

#endif

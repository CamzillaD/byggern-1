#ifndef PID_REGULATOR_H
#define PID_REGULATOR_H

void pid_regulator_init(double k_p, double k_i, double k_d);

double pid_regulator_get_u(double set_point, double sample);

#endif

#include "pid_regulator.h"
#include "sam.h"
#include <stdint.h>

/* This PID regulator implementation is based on */
/* the paper "PID Controller Using the TMS320C31 DSK for */
/* Real-Time DC Motor Speed and Position Control" by */
/* Jianxin Tang and Rulph Chassaing */

/* static double m_a0, m_a1, m_a2; */
/* static double m_last_sample; */

#define PID_ERROR_SCALE 0.0001
#define PID_D_HORIZON 10

static double m_kp, m_ki, m_kd;
static double m_euler_integral;
static double m_max_integral;
static double m_time_step;
static double m_last_sample;

void pid_regulator_init(double k_p, double k_i, double k_d){
    m_kp = k_p;
    m_ki = k_i;
    m_kd = k_d;

    m_euler_integral = 0.0;
    m_max_integral = 100.0;
    m_time_step = 5.0;

    m_last_sample = 0.0;


    /* double k_p = pid_config.k_p; */
    /* double k_i = pid_config.k_i; */
    /* double k_d = pid_config.k_d; */
    /* double t = pid_config.sample_time; */

    /* m_a0 = k_p + (k_i * t / 2.0) + k_d / 2.0; */
    /* m_a1 = -k_p + (k_i * t / 2.0) - (2.0 * k_d / t); */
    /* m_a2 = k_d / t; */

    /* /1* Enable peripheral clock for Timer Counter 0 *1/ */
    /* PMC->PMC_PCER0 = PMC_PCER0_PID27; */

    /* NVIC_EnableIRQ(ID_TC0); */

    /* /1* Clock: MCK / 128 *1/ */
    /* TC0->TC_CMR0 = TC_CMR0_TIMER_CLOCK4 */
    /*              | TC_CMR0_BURST_NONE */
    /*              | TC_CMR0_ETRGEDG_NONE */
    /*              ; */

    /* /1* Enable interrupt *1/ */
    /* TC0->TC_IER0 = */

    /* /1* Enable clock, reset counter *1/ */
    /* TC0->TC_CCR0 = TC_CCR0_CLKEN | TC_CCR0_SWTRG; */
}

double pid_regulator_get_u(double set_point, double sample){
    double error = (set_point - sample) * PID_ERROR_SCALE;
    m_euler_integral += error;

    double p_term = m_kp * error;

    double i_term = 0;
    if(abs(m_euler_integral) < m_max_integral){
        i_term = m_ki * m_euler_integral;
    }
    else if(m_euler_integral < 0){
        i_term = -m_ki * m_max_integral;
        m_euler_integral = -m_max_integral;
    }
    else{
        i_term = m_ki * m_max_integral;
        m_euler_integral = m_max_integral;
    }

    static uint8_t d_horizon = PID_D_HORIZON;
    static double d_term = 0;
    d_horizon--;

    if(d_horizon == 0){
        d_horizon = PID_D_HORIZON;
        d_term = (sample - m_last_sample);
        m_last_sample = sample;
    }

    return p_term + i_term * m_time_step - d_term / m_time_step;

/*     double num = m_a0 + m_a1 * sample + m_a2 * m_last_sample; */
/*     double den = 1 - sample; */
/*     m_last_sample = sample; */
/*     return num * (set_point - sample) / den; */
}

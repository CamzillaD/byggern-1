#include "motor_position.h"
#include "motor.h"
#include "encoder.h"
#include "pid_regulator.h"

#define SYSTICK_CTRL (*((uint32_t *)(0xe000e010)))
#define SYSTICK_LOAD (*((uint32_t *)(0xe000e014)))

#define SYSTICK_CTRL_MCK_TICKINT_EN 0x07
#define SYSTICK_CTRL_MCK_DIS 0x04
#define SYSTICK_LOAD_20_ms 0x19a280

#define MOTOR_SPEED_HALT 127
#define MOTOR_SPEED_LEFT 84
#define MOTOR_SPEED_RIGHT 171

#define ENCODER_SAFETY_MARGIN 500.0

static inline void m_delay_min_1ms(){
    for(uint32_t i = 0; i < 6167; i++){
        __asm__("nop;");
    }
}

static inline void m_delay_approx_ms(uint32_t ms){
    for(uint32_t i = 0; i < ms; i++){
        m_delay_min_1ms();
    }
}

static uint16_t m_encoder_max;
static uint16_t m_reference;

static Pid m_pid;


void SysTick_Handler(){
    int32_t u = pid_regulator_step(&m_pid, m_reference, encoder_read());

    motor_command_speed(u);
}

void motor_position_init(){
    /* Calibrate encoder */
    motor_command_speed(MOTOR_SPEED_RIGHT);
    m_delay_approx_ms(2500);
    encoder_reset();

    motor_command_speed(MOTOR_SPEED_LEFT);
    m_delay_approx_ms(2500);
    m_encoder_max = encoder_read();

    motor_command_speed(MOTOR_SPEED_HALT);

    /* Create PID */
    m_pid = pid_regulator_new();

    m_pid.k_p = -0.05;

    m_pid.k_i = -0.01;
    m_pid.integral_min = -15.0;
    m_pid.integral_max =  15.0;

    m_pid.k_d = -0.002;
    m_pid.tau = 0.005;

    m_pid.u_min = MOTOR_SPEED_LEFT;
    m_pid.u_max = MOTOR_SPEED_RIGHT;

    m_pid.u_bias = 127.0;

    /* 20 ms SysTick interrupt */
    m_pid.t = 0.020;
}

void motor_position_set_reference(uint8_t position){
    int32_t reference = (m_encoder_max * position) / 255;
    reference = m_encoder_max - reference;

    if(reference < 0){
        reference = 0;
    }

    reference &= 0xffff;

    m_reference = reference;
}

void motor_position_tracking_enable(){
    SYSTICK_LOAD = SYSTICK_LOAD_20_ms;
    SYSTICK_CTRL = SYSTICK_CTRL_MCK_TICKINT_EN;
}

void motor_position_tracking_disable(){
    SYSTICK_CTRL = SYSTICK_CTRL_MCK_DIS;
}
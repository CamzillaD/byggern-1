#include "motor.h"
#include "sam.h"
#include "pid_regulator.h"
#include "pid_regulator_clock.h"
#include "real_time.h"

#include "uart_and_printf/printf-stdarg.h"

static uint16_t m_encoder_max_value;
static uint8_t m_encoder_calibrated = 0;

static Pid m_position_pid;
static double m_position_set_point;
static double m_position_sample;

static uint8_t m_position_mode_enable = 1;

uint16_t motor_encoder_read(){
    uint16_t encoder = 0;

    REG_PIOD_CODR = PIO_CODR_P0 | PIO_CODR_P2;
    real_time_delay_ms(1);

    encoder |= (REG_PIOC_PDSR & 0x1fe) << 7;

    REG_PIOD_SODR = PIO_SODR_P2;
    real_time_delay_ms(1);

    encoder |= (REG_PIOC_PDSR & 0x1fe) << 1;

    REG_PIOD_SODR = PIO_SODR_P0;

    //printf("%d \n\r", encoder);
    return encoder;
}

static void motor_encoder_calibrate(){
    m_encoder_calibrated = 0;

    motor_command_speed(170);
    printf("go right \n\r");
    real_time_delay_ms(1000);

    /* Reset encoder */
    REG_PIOD_CODR |= PIO_SODR_P1;
    real_time_delay_ms(100);
    REG_PIOD_SODR |= PIO_SODR_P1;

    motor_command_speed(95);
    printf("go left \n\r");
    real_time_delay_ms(1000);

    m_encoder_max_value = motor_encoder_read();

    motor_command_speed(0x7f);
    printf("stop \n\r");

    m_encoder_calibrated = 1;
}


void motor_init(Pid motor_position_pid){
    m_position_pid = motor_position_pid;

    pid_regulator_clock_init(
        &m_position_pid,
        &m_position_set_point,
        &m_position_sample
    );

    /* Initialize DACC */
    PMC->PMC_PCER1 = PMC_PCER1_PID38;
    DACC->DACC_WPMR &= ~DACC_WPMR_WPEN;
    DACC->DACC_CHER =DACC_CHER_CH1;
    DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL1;
    DACC->DACC_WPMR = DACC_WPMR_WPEN;

    /* Initialize Encoder */
    REG_PMC_PCER0 = PMC_PCER0_PID14 | PMC_PCER0_PID13;

    uint32_t input_mask = PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P4
                        | PIO_PER_P5 | PIO_PER_P6 | PIO_PER_P7 | PIO_PER_P8;

    REG_PIOC_PER = input_mask;
    REG_PIOC_ODR = input_mask;

    REG_PIOC_PUER = input_mask;

    REG_PIOD_PER = PIO_PER_P0 | PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P9 | PIO_PER_P10;
    REG_PIOD_OER = PIO_PER_P0 | PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P9 | PIO_PER_P10;

    REG_PIOD_CODR = PIO_ODR_P9;
    REG_PIOD_SODR = PIO_SODR_P0 | PIO_SODR_P1;

    REG_PIOD_SODR = PIO_SODR_P1;

    /* Enable motor */
    REG_PIOD_SODR = PIO_PD9;

    motor_encoder_calibrate();
}

void motor_command_speed(uint8_t speed){
    if(speed > 124 && speed < 131){
        DACC->DACC_CDR = 0x000;
    }
    else if(speed <= 124){
        REG_PIOD_SODR = PIO_SODR_P10;
        DACC->DACC_CDR = -33 * speed + 4092;
    }
    else if(speed >= 131){
        REG_PIOD_CODR = PIO_CODR_P10;
        DACC->DACC_CDR = 33 * speed -4323;
    }

    m_position_mode_enable = 0;
}

void motor_command_position(uint8_t position){
    m_position_set_point = position * m_encoder_max_value / 0xff;
    m_position_sample = motor_encoder_read();

    /* Account for inaccuracies after encoder reset */
    if(m_position_sample > m_encoder_max_value){
        m_position_sample = 0.0;
    }

    motor_command_speed(m_position_pid.u);

    m_position_mode_enable = 1;
}

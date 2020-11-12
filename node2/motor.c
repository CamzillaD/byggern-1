#include "motor.h"
#include "sam.h"
#include "pid_regulator.h"

#include "uart_and_printf/printf-stdarg.h"

static uint16_t m_encoder_max_value;
static uint8_t m_encoder_calibrated = 0;
static Pid m_position_pid;

void motor_delay(uint32_t ms_approx){
    ms_approx *= 139;
    while(ms_approx--){
        __asm__("nop;");
    }
}

uint16_t motor_encoder_read(){
    uint16_t encoder = 0;

    REG_PIOD_CODR = PIO_CODR_P0 | PIO_CODR_P2;
    motor_delay(1);

    encoder |= (REG_PIOC_PDSR & 0x1fe) << 7;

    REG_PIOD_SODR = PIO_SODR_P2;
    motor_delay(1);

    encoder |= (REG_PIOC_PDSR & 0x1fe) << 1;

    REG_PIOD_SODR = PIO_SODR_P0;

    return encoder;
}

static void motor_encoder_calibrate(){
    m_encoder_calibrated = 0;

    uint8_t same_sample_counter = 0;
    uint16_t encoder = 1;
    uint16_t last_encoder = 0;

    motor_command_speed(0xff);
    while(same_sample_counter < 150){
        encoder = motor_encoder_read();
        if(encoder == last_encoder){
            same_sample_counter++;
        }
        else{
            same_sample_counter = 0;
        }
    }

    same_sample_counter = 0;

    /* Reset encoder */
    REG_PIOD_CODR |= PIO_SODR_P1;
    motor_delay(100);
    REG_PIOD_SODR |= PIO_SODR_P1;

    motor_command_speed(0x00);
    while(same_sample_counter < 150){
        encoder = motor_encoder_read();
        if(encoder == last_encoder){
            same_sample_counter++;
        }
        else{
            same_sample_counter = 0;
        }
    }

    m_encoder_max_value = motor_encoder_read();

    motor_command_speed(0x7f);

    m_encoder_calibrated = 1;
}


void motor_init(Pid motor_position_pid){
    m_position_pid = motor_position_pid;

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
        DACC->DACC_CDR = 33 * speed;
    }
    else if(speed >= 131){
        REG_PIOD_CODR = PIO_CODR_P10;
        DACC_DACC_CDR = 33 * speed - 4326;
    }
}

void motor_command_position(uint8_t position){
    double set_point = (m_encoder_max_value / 200.0) * (position + 100);
    double sample = motor_encoder_read();
    printf("%d \n\r", (uint32_t)(sample));

    /* Account for inaccuracies after encoder reset */
    if(sample > m_encoder_max_value){
        sample = 0.0;
    }

    double u = pid_step(&m_position_pid, set_point, sample);

    motor_command_speed(u);
}

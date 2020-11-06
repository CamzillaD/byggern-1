#include "motor.h"
#include "sam.h"
#include "pid_regulator.h"



#include "uart_and_printf/printf-stdarg.h"

static uint16_t m_encoder_max_value;
static Pid m_position_pid;

static void motor_delay(uint32_t ms_approx){
    ms_approx *= 139;
    while(ms_approx--){
        __asm__("nop;");
    }
}

static void motor_dac_init(){
    PMC->PMC_PCER1 = PMC_PCER1_PID38; //enable clock
    DACC->DACC_WPMR &= ~DACC_WPMR_WPEN;
    DACC->DACC_CHER =DACC_CHER_CH1;
    DACC->DACC_MR = DACC_MR_USER_SEL_CHANNEL1;
    DACC->DACC_WPMR = DACC_WPMR_WPEN;
}

static void motor_encoder_reset(){
     //toggle !RST to reset encoder
     REG_PIOD_CODR |= PIO_SODR_P1;
     motor_delay(100);
     REG_PIOD_SODR |= PIO_SODR_P1;
}

static void motor_encoder_init(){
    motor_command_speed(0x08);
    motor_delay(70000);
    motor_encoder_reset();


    motor_command_speed(-0x08);
    motor_delay(70000);

    m_encoder_max_value = motor_encoder_read();

    motor_command_speed(0x00);
}

static void motor_enable(){
    REG_PIOD_SODR = PIO_PD9;
}

static void motor_disable(){
    REG_PIOD_CODR = PIO_SODR_P9;
}

static void motor_position_pid_init(){
    m_position_pid = pid_new();

    m_position_pid.k_p = 1;
    m_position_pid.k_i = 0.05;
    m_position_pid.k_d = 0.3;

    m_position_pid.t = 0.01;
    m_position_pid.tau = 10;

    m_position_pid.integral_min = -0.1;
    m_position_pid.integral_min =  0.1;

    m_position_pid.u_min = -10;
    m_position_pid.u_max =  10;
}


void motor_init(){
    motor_dac_init();

    REG_PMC_PCER0 = PMC_PCER0_PID14 | PMC_PCER0_PID13 ; //enable clock

 //   REG_PIOC_WPMR = 0x50494f00;
 //   REG_PIOD_WPMR = 0x50494f00;

    uint32_t input_mask = PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P3 | PIO_PER_P4
                        | PIO_PER_P5 | PIO_PER_P6 | PIO_PER_P7 | PIO_PER_P8;

    REG_PIOC_PER = input_mask; //enable pc1-pc8 = DO0-D07 => encoder counter value
    REG_PIOC_ODR = input_mask; //output disable = input

    REG_PIOC_PUER = input_mask;

    REG_PIOD_PER = PIO_PER_P0 | PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P9 |  PIO_PER_P10; //Enable pin
    REG_PIOD_OER = PIO_PER_P0 | PIO_PER_P1 | PIO_PER_P2 | PIO_PER_P9 |  PIO_PER_P10; //Enable output

    REG_PIOD_CODR = PIO_ODR_P9; //set enable low
    REG_PIOD_SODR = PIO_SODR_P0 | PIO_SODR_P1; //restes, and OE pin high, no value on MJ2-pins

    REG_PIOD_SODR = PIO_SODR_P1; //reset til høy, slik at den ikke resetter

    motor_enable();

    motor_encoder_init();
}

//Må man sjekke txrdy-flagget før man skrver til reg?
void motor_command_speed(int16_t data){
    //uint8_t speed = data;
    if (data < 0){
        REG_PIOD_SODR = PIO_SODR_P10; //går mot venstre
    }
    else{
        REG_PIOD_CODR = PIO_CODR_P10; //går mot høyre
    }
    DACC->DACC_CDR = (abs(data) << 7);
}

void motor_command_position(int16_t position){
    double set_point = (m_encoder_max_value / 200.0) * (position + 100);
    double sample = motor_encoder_read();

    /* Account for inaccuracies after encoder reset */
    if( encoder > m_encoder_max_value){
        encoder = 0.0;
    }

    double u = pid_step(&m_position_pid, set_point, sample);

    motor_command_speed(u);
}

uint16_t motor_encoder_read(){
    uint16_t data = 0;

    REG_PIOD_CODR |= PIO_SODR_P0; //sets !OE, can read MJ2-pins
    PIOD->PIO_CODR = PIO_CODR_P0;
    REG_PIOD_CODR |= PIO_SODR_P2; //sel low

    //wait 20 microsek
    motor_delay(1);
    data |= (REG_PIOC_PDSR & 0x1FE) << 7; //data status register = data

   REG_PIOD_SODR = PIO_SODR_P2; //Sel high

    motor_delay(1);
    data |= (REG_PIOC_PDSR & 0x1FE) << 1;

    //toggle !RST to reset encoder

    REG_PIOD_SODR |= PIO_SODR_P0; //set OE high


    return data;
}

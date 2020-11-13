#include "encoder.h"
#include "sam.h"

static uint16_t m_encoder_max_value;

static inline void m_delay_min_30us(){
    for(uint32_t i = 0; i < 185; i++){
        __asm__("nop;");
    }
}

static inline void m_output_enable(){
    PIOD->PIO_CODR = PIO_CODR_P0;
}

static inline void m_output_disable(){
    PIOD->PIO_SODR = PIO_SODR_P0;
}

static inline void m_reset_enable(){
    PIOD->PIO_CODR = PIO_CODR_P1;
}

static inline void m_reset_disable(){
    PIOD->PIO_SODR = PIO_SODR_P1;
}

static inline void m_byte_high(){
    PIOD->PIO_CODR = PIO_CODR_P2;
}

static inline void m_byte_low(){
    PIOD->PIO_SODR = PIO_SODR_P2;
}

void encoder_init(){
    /* Enable clock for PIOC */
    PMC->PMC_PCER0 = PMC_PCER0_PID13;

    /* Encoder input pins */
    PIOC->PIO_PER = 0x1fe;
    PIOC->PIO_ODR = 0x1fe;

    /* Encoder control pins */
    PIOD->PIO_PER = PIO_PER_P0 | PIO_PER_P1 | PIO_PER_P2;
    PIOD->PIO_OER = PIO_PER_P0 | PIO_PER_P1 | PIO_PER_P2;

    m_reset_disable();
    m_output_disable();
}

void encoder_reset(){
    m_reset_enable();
    m_delay_min_30us();
    m_reset_disable();
}

uint16_t encoder_read(){
    uint16_t encoder = 0;

    m_byte_high();
    m_output_enable();

    m_delay_min_30us();

    encoder = ((PIOC->PIO_PDSR & 0x1fe) << 7);

    m_byte_low();

    m_delay_min_30us();

    encoder |= ((PIOC->PIO_PDSR & 0x1fe) >> 1);

    m_output_disable();

    return encoder;
}
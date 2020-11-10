#include "score_counter.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static uint16_t m_octet_score;

ISR(TIMER1_COMPA_vect){
    m_octet_score++;
}

void score_counter_init(){
    /* Disconnect Timer/Counter1 from pins */
    TCCR1A = 0x00;

    /* Clear timer on compare, 64 prescaler */
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);

    /* 8.0 Hz timer frequency */
    OCR1A = 31249;

    /* Enable interrupt on timer compare */
    TIMSK |= (1 << OCIE1A);
}

void score_counter_start(){
    TCNT1 = 0;
    m_octet_score = 0;
}

uint16_t score_counter_end(){
    return m_octet_score;
}

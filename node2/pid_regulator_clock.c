#include "pid_regulator_clock.h"
#include "sam.h"

#include <stdint.h>
#include "uart_and_printf/printf-stdarg.h"
static uint32_t m_hit = 0;

void TC1_Handler(){
    printf("%6d\n\r", m_hit++);
}

void pid_regulator_clock_enable(){
    /* Enable peripheral power */
    PMC->PMC_PCR0 = PMC_PCR0_PID28;

    /* Enable peripheral clock */
    TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

    /* Master clock / 2, Reset on RC compare */
    TC1->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_TIMER_CLOCK0
                              | TC_CMR_CPCTRG
                              ;

    /* Timer period of 1.0 s */
    /* TC1->TC_CHANNEL[0].TC_RC = 0x0280de80; */

    /* Timer period of 1.0 ms */
    /* TC1->TC_CHANNEL[0].TC_RC = 42000; */

    /* Timer period of 0.1 ms */
    TC1->TC_CHANNEL[0].TC_RC = 4200;

    /* Enable interrupt on RC compare */
    /* TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS; */
}

void pid_regulator_clock_disable(){
    /* Disable interrupt on RC compare */
    TC1->TC_CHANNEL[0].TC_IDR = TC_IDR_CPCS;
}

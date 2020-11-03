#include "network.h"
#include "mcp2518fd.h"
#include "connection_indicator.h"
#include "uart.h"
#include <avr/interrupt.h>
#include <avr/wdt.h>

#define NETWORK_BUFFER_SIZE 16
#define NETWORK_FLAG 0x02

typedef enum {
    NETWORK_AWAIT_FLAG,
    NETWORK_AWAIT_EVENT,
    NETWORK_AWAIT_VALUE,
    NETWORK_AWAIT_SUM,
} NetworkAwait;

static NetworkAwait m_await = NETWORK_AWAIT_FLAG;
static uint8_t m_read_event;
static uint8_t m_read_value;

void static network_write(uint8_t event, uint8_t value){
    uart_send(NETWORK_FLAG);
    uart_send(event);
    uart_send(value);
    uart_send(event + value);
}

void static network_enact(){
    switch(m_read_event){
        case NETWORK_EVENT_INDICATOR:
            if(m_read_value){
                connection_indicator_turn_on();
            }
            else{
                connection_indicator_turn_off();
            }
            break;

        case NETWORK_EVENT_REQUEST_RESET:
            wdt_enable(WDTO_15MS);
            while(1);
            break;
    }
}

ISR(USART0_RXC_vect){
    uint8_t byte = uart_recv();

    switch(m_await){
        case NETWORK_AWAIT_FLAG:
            if(byte == NETWORK_FLAG){
                m_await = NETWORK_AWAIT_EVENT;
            }
            break;

        case NETWORK_AWAIT_EVENT:
            m_read_event = byte;
            m_await = NETWORK_AWAIT_VALUE;
            break;

        case NETWORK_AWAIT_VALUE:
            m_read_value = byte;
            m_await = NETWORK_AWAIT_SUM;
            break;

        case NETWORK_AWAIT_SUM:
            if(m_read_event + m_read_value == byte){
                network_enact();
            }
            m_await = NETWORK_AWAIT_FLAG;
            break;
    }
}

void network_write_joystick(Joystick * p_left, Joystick * p_right){
    network_write(NETWORK_EVENT_JOYSTICK_LH, p_left->x);
    network_write(NETWORK_EVENT_JOYSTICK_LV, p_left->y);
    if(p_left->position_changed){
        network_write(NETWORK_EVENT_JOYSTICK_LP, p_left->position);
    }

    network_write(NETWORK_EVENT_JOYSTICK_RH, p_right->x);
    network_write(NETWORK_EVENT_JOYSTICK_RV, p_right->y);
    if(p_right->position_changed){
        network_write(NETWORK_EVENT_JOYSTICK_RP, p_right->position);
    }
}

void network_write_can_interrupt(){
    network_write(NETWORK_EVENT_CAN_INTERRUPT, 0x01);
}

void network_write_generic(uint8_t value){
    network_write(NETWORK_EVENT_GENERIC, value);
}

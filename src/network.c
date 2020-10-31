#include "network.h"
#include "mcp2518fd.h"
#include "uart.h"
#include <avr/interrupt.h>

#define NETWORK_BUFFER_SIZE 16
#define NETWORK_FLAG 0x02

typedef enum {
    NETWORK_AWAIT_FLAG,
    NETWORK_AWAIT_KEY,
    NETWORK_AWAIT_VALUE,
    NETWORK_AWAIT_SUM,
} NetworkAwait;

static NetworkAwait m_await = NETWORK_AWAIT_FLAG;
static NetworkMessage m_buffer[NETWORK_BUFFER_SIZE];
static uint8_t m_write_head = 0;
static uint8_t m_read_head = 0;

#define MESSAGE_SUM(x) ((m_buffer[x].key + m_buffer[x].value))

ISR(USART0_RXC_vect){
    uint8_t byte = uart_recv();

    switch(m_await){
        case NETWORK_AWAIT_FLAG:
            if(byte == NETWORK_FLAG){
                m_await = NETWORK_AWAIT_KEY;
            }
            break;

        case NETWORK_AWAIT_KEY:
            m_buffer[m_write_head].key = byte;
            m_await = NETWORK_AWAIT_VALUE;
            break;

        case NETWORK_AWAIT_VALUE:
            m_buffer[m_write_head].value = byte;
            m_await = NETWORK_AWAIT_SUM;
            break;

        case NETWORK_AWAIT_SUM:
            if(MESSAGE_SUM(m_write_head) == byte){
                m_write_head++;
                m_write_head %= NETWORK_BUFFER_SIZE;
            }
            m_await = NETWORK_AWAIT_FLAG;
            break;
    }
}

uint8_t network_message_read(NetworkMessage * p_message){
    if(m_read_head == m_write_head){
        return 0;
    }

    *p_message = m_buffer[m_read_head];

    m_read_head++;
    m_read_head %= NETWORK_BUFFER_SIZE;

    return 1;
}

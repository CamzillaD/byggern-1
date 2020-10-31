#include "network.h"
#include "mcp2518fd.h"
#include "uart.h"
#include <avr/interrupt.h>

#define NETWORK_BUFFER_SIZE 32
#define NETWORK_STX 0x02
#define NETWORK_ETX 0x03

typedef enum {
    NETWORK_READ_STX,
    NETWORK_READ_LEN,
    NETWORK_READ_PAYLOAD,
    NETWORK_READ_SUM,
    NETWORK_READ_ETX
} NetworkReadState;

static NetworkReadState m_read_state = NETWORK_READ_STX;
static uint8_t m_read_buffer[NETWORK_BUFFER_SIZE];
static uint8_t m_read_buffer_index = 0;

static uint8_t network_sum_buffer(){
    uint8_t sum = 0;
    for(uint8_t i = 0; i < m_read_buffer_index; i++){
        sum += m_read_buffer[i];
    }
    return sum;
}

ISR(USART0_RXC_vect){
    uint8_t byte = uart_recv();

    switch(m_read_state){
        case NETWORK_READ_STX:
            if(byte == NETWORK_STX){
                m_read_state = NETWORK_READ_LEN;
            }
            break;

        case NETWORK_READ_LEN:
            m_read_buffer[0] = byte;
            m_read_buffer_index = 1;
            m_read_state = NETWORK_READ_PAYLOAD;
            break;

        case NETWORK_READ_PAYLOAD:
            m_read_buffer[m_read_buffer_index++] = byte;
            if(m_read_buffer_index > m_read_buffer[0]){
                m_read_state = NETWORK_READ_SUM;
            }
            break;

        case NETWORK_READ_SUM:
            if(network_sum_buffer() == byte){
                m_read_state = NETWORK_READ_ETX;
            }
            else{
                m_read_state = NETWORK_READ_STX;
            }
            break;

        case NETWORK_READ_ETX:

            m_read_state = NETWORK_READ_STX;
            break;
    }
}

void network_init(){
    /* Enable MCP2518FD GPIO1 (Disable ~INT1) */
    mcp2518fd_write(MCP_SFR_IOCON(3), 0x02);

    /* Use MCP2518FD GPIO1 as output */
    mcp2518fd_write(MCP_SFR_IOCON(0), 0x00);
}

uint8_t network_message_read(){
    return 1;
}

void network_indicate(NetworkState state){
    switch(state){
        case NETWORK_STATE_CONNECTED:
            mcp2518fd_write(MCP_SFR_IOCON(1), 0x00);
            break;

        case NETWORK_STATE_DISCONNECTED:
            mcp2518fd_write(MCP_SFR_IOCON(1), 0x02);
            break;
    }
}

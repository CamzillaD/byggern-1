#include "can.h"
#include "mcp2515.h"

#include "uart.h"

#include <avr/interrupt.h>

static uint8_t recv_flag;
static CanFrame recv_frame;

ISR(INT0_vect){
    
    recv_flag = 1;
 
    recv_frame.id = (mcp_read(MCP_RXB0SIDH) << 3);
    recv_frame.id |= (mcp_read(MCP_RXB0SIDL) >> 5);

    recv_frame.size = mcp_read(MCP_RXB0DLC) & 0x0f;
    
    
    for (int i = 0; i < recv_frame.size; i++){
        recv_frame.buffer[i] = mcp_read(MCP_RXB0DM + i);
    }

    uint8_t mcp_canintf = 0x00;
    mcp_write(MCP_CANINTF, mcp_canintf);
}


void can_init(){
    /* INT0 pin input */
    DDRD &= ~(1 << PD2);

    /* Trigger INT0 on falling edge */
    MCUCR |= (1 << ISC01);

    /* Enable INT0 */
    GICR |= (1 << INT0);

    uint8_t mcp_config
        = MCP_CANCTRL_CONFIG
        | MCP_CANCTRL_PRESCALE_8
        | MCP_CANCTRL_CLKOUT
        ;

    mcp_write(MCP_CANCTRL, mcp_config);

    /* CAN Bit timing */
    uint8_t mcp_cnf3 = (0x01);
    mcp_write(MCP_CNF3, mcp_cnf3);

    uint8_t mcp_cnf2 = (0xb5);
    mcp_write(MCP_CNF2, mcp_cnf2);

    uint8_t mcp_cnf1 = (0x43);
    mcp_write(MCP_CNF1, mcp_cnf1);

    uint8_t mcp_caninte = 0x01;
    mcp_write(MCP_CANINTE, mcp_caninte);   

    mcp_config
        = MCP_CANCTRL_NORMAL
        | MCP_CANCTRL_PRESCALE_8
        | MCP_CANCTRL_CLKOUT
        ;

    mcp_write(MCP_CANCTRL, mcp_config);

    uint8_t mcp_rxb0_config = 0x60;
    mcp_write(MCP_RXB0CTRL, mcp_rxb0_config);
}

#define F_CPU 4915200
#include <util/delay.h>

void can_write(const CanFrame * p_frame){
    mcp_write(MCP_TXB0SIDH, (uint8_t)(p_frame->id >> 3));
    mcp_write(MCP_TXB0SIDL, (uint8_t)(p_frame->id << 5));

    mcp_write(MCP_TXB0DLC, p_frame->size & 0x0f);

    for(uint8_t i = 0; i < p_frame->size; i++){
        mcp_write(MCP_TXB0D0 + i, p_frame->buffer[i]);
    }

    mcp_bit_modify(MCP_TXB0CTRL, MCP_TXB_TX_REQUEST, 1);

    _delay_ms(40);
}

uint8_t can_read(CanFrame * p_frame){
    if (recv_flag == 0){
        return 0;
    }

    cli();

    p_frame->id = recv_frame.id;
    p_frame->size = recv_frame.size;

    for (int i = 0; i < p_frame->size; i++){
        p_frame->buffer[i] = recv_frame.buffer[i];
    }

    recv_flag = 0;

    sei();

    return 1;
}
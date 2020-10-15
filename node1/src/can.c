#include "can.h"
#include "mcp2515.h"
#include <avr/interrupt.h>

#include "uart.h"

//static gjør denne modul-bundet, lenkern kan ikke lenke den fra noe annet sted
static uint8_t recv_flag;
static CanFrame recv_frame;
static uint8_t recv_buffer[8];


ISR(INT0_vect){

    uart_send('s');
    uart_send('\n');
    uart_send('\r');
    

    recv_flag = 1;
 
    recv_frame.id = (mcp_read(MCP_RXB0SIDH) << 3);
    recv_frame.id |= (mcp_read(MCP_RXB0SIDL) >> 5);
    
    for (int i = 0; i < recv_frame.size; i++){
        recv_frame.buffer[i] = mcp_read(MCP_RXB0DM + i);
    }
}


void can_init(){
    cli();
    recv_frame.buffer = recv_buffer;

    // INT0 pin input
    DDRD &= ~(1 << PD2);

    // Trigger INT0 on falling edge
    MCUCR |= (1 << ISC01);

    // Enable INT0
    GICR |= (1 << INT0);

    mcp_init();

    uint8_t mcp_config
        = MCP_CANCTRL_NORMAL
        | MCP_CANCTRL_PRESCALE_8
        | MCP_CANCTRL_CLKOUT
        ;
    mcp_write(MCP_CANCTRL, mcp_config);

    uint8_t mcp_rxb0_config = 0x60;
    mcp_write(MCP_RXB0CTRL, mcp_rxb0_config);

    //setter bit timing
    uint8_t mcp_cnf1 = 0x00 | 0x14;
    mcp_write(MCP_CNF1, mcp_cnf1);
    uint8_t mcp_cnf2 = 0x80 | 0x18 | 0x03;
    mcp_write(MCP_CNF2, mcp_cnf2);
    uint8_t mcp_cnf3 = 0x03;
    mcp_write(MCP_CNF3, mcp_cnf3);

    sei();
}

void can_send(const CanFrame * p_frame){

    mcp_write(MCP_TXB0SIDH, (uint8_t)(p_frame->id >> 3));
    mcp_write(MCP_TXB0SIDL, (uint8_t)(p_frame->id << 5));

    mcp_write(MCP_TXB0DLC, p_frame->size & 0x0f);

    for(uint8_t i = 0; i < p_frame->size; i++){
        mcp_write(MCP_TXB0D0 + i, p_frame->buffer[i]);
    }

    mcp_bit_modify(MCP_TXB0CTRL, MCP_TXB_TX_REQUEST, 1);
}

uint8_t can_recv(CanFrame * p_frame){
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
 
uint8_t can_test(){
    return mcp_read(0x2c);
}




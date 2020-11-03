#include "can.h"
#include "mcp2518fd.h"
#include <avr/interrupt.h>

#include "uart.h"
#include "network.h"
#include "connection_indicator.h"

ISR(INT2_vect){
    network_write_can_interrupt();
    connection_indicator_turn_on();
}

void can_init(){
    /* Disable INT2 interrupt */
    GICR &= ~(1 << INT2);

    /* PE0 input with internal pullup */
    DDRE &= ~(1 << PE0);
    PORTE |= (1 << PE0);

    /* Trigger INT2 on falling edge */
    EMCUCR &= ~(1 << ISC2);

    /* Enable INT2 interrupt */
    GICR |= (1 << INT2);


    /* Enter configuration mode */
    mcp2518fd_sfr_write(MCP_SFR_C1CON(3), 0x04);
    /* Enable transmit queue */
    mcp2518fd_sfr_write(MCP_SFR_C1CON(2), 0x10);

    /* Disable error correction module */
    mcp2518fd_sfr_write(MCP_SFR_ECCCON(0), 0x00);

    /* Nominal bit time configuration */
    /* mcp2518fd_sfr_write(MCP_SFR_C1NBTCFG(...), ...); */

    /* Data bit time configuration */
    /* mcp2518fd_sfr_write(MCP_SFR_C1DBTCFG(...), ...); */

    /* Transmit queue: 8 data byte payload, 8 messages deep */
    mcp2518fd_sfr_write(MCP_SFR_C1TXQCON(3), 0x07);
    /* Transmit queue: Unlimited attempts, highest priority */
    mcp2518fd_sfr_write(MCP_SFR_C1TXQCON(2), 0x7f);
    /* Transmit queue: No interrupts */
    mcp2518fd_sfr_write(MCP_SFR_C1TXQCON(0), 0x00);

    /* FIFO1: 8 data byte payload, 16 messages deep */
    mcp2518fd_sfr_write(MCP_SFR_C1FIFOCON(1, 3), 0x0f);
    /* FIFO1: Receive FIFO, no timestamp, no interrupts */
    mcp2518fd_sfr_write(MCP_SFR_C1FIFOCON(1, 2), 0x00);

    /* Filter 0 disabled */
    mcp2518fd_sfr_write(MCP_SFR_C1FLTCON(0, 0), 0x00);
    /* Filter 0 accepts only standard frames, all IDs */
    mcp2518fd_sfr_write(MCP_SFR_FLTOBJ(0, 3), 0x00);
    mcp2518fd_sfr_write(MCP_SFR_C1MASK(0, 3), 0x40);
    mcp2518fd_sfr_write(MCP_SFR_C1MASK(0, 2), 0x00);
    mcp2518fd_sfr_write(MCP_SFR_C1MASK(0, 1), 0x00);
    mcp2518fd_sfr_write(MCP_SFR_C1MASK(0, 0), 0x00);
    /* Filter 0 enabled and puts messages in FIFO1 */
    mcp2518fd_sfr_write(MCP_SFR_C1FLTCON(0, 0), 0x81);

    /* Interrupt on receive FIFO */
    /* mcp2518fd_sfr_write(MCP_SFR_C1INT(2), 0x02); */
    /* Interrupt on transmission */
    mcp2518fd_sfr_write(MCP_SFR_C1INT(2), 0x01);

    /* Internal loop-back mode */
    mcp2518fd_sfr_write(MCP_SFR_C1CON(3), 0x02);

    /* CAN 2.0 Normal mode */
    /* mcp2518fd_sfr_write(MCP_SFR_C1CON(3), 0x06); */
}

uint8_t can_send(const CanFrame * p_frame){
    /* Check if there is space in the transmit queue */
    if(!(mcp2518fd_sfr_read(MCP_SFR_C1TXQSTA(0)) & 0x01)){
        return 1;
    }

    /* Get transmit queue user address */
    uint16_t ua = 0;
    /* ua |= mcp2518fd_sfr_read(MCP_SFR_C1TXQUA(3)) << 24; */
    /* ua |= mcp2518fd_sfr_read(MCP_SFR_C1TXQUA(2)) << 16; */
    ua |= mcp2518fd_sfr_read(MCP_SFR_C1TXQUA(1)) << 8;
    ua |= mcp2518fd_sfr_read(MCP_SFR_C1TXQUA(0));
    ua += 0x400;


    uint8_t transmit_object_header[8] = {
        0x00, 0x00,
        ((p_frame->id >> 8) & 0x07), (uint8_t)(p_frame->id),
        0x00, 0x00,
        0x00, (p_frame->size & 0x0f)
    };

    mcp2518fd_mem_write(ua +  0, transmit_object_header + 0);
    mcp2518fd_mem_write(ua +  4, transmit_object_header + 4);
    mcp2518fd_mem_write(ua +  8, p_frame->buffer + 0);
    mcp2518fd_mem_write(ua + 12, p_frame->buffer + 4);

    /* Request send and increment user address */
    mcp2518fd_sfr_write(MCP_SFR_C1TXQCON(1), 0x03);

    return 0;
}

uint8_t can_recv(CanFrame * p_frame){
}

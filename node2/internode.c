#include "internode.h"
#include "can_controller.h"
#include "system_reset.h"
#include "sam.h"

InternodeCommand g_COMMAND;

/* Messages this module sends */
#define INTERNODE_ID_END_GAME     0x09

/* Messages this module receives */
#define INTERNODE_ID_RESET        0x05
#define INTERNODE_ID_SOLENOID     0x18
#define INTERNODE_ID_POSITION     0x13
#define INTERNODE_ID_SPEED        0x12
#define INTERNODE_ID_SERVO        0x16

static CAN_MESSAGE m_frame;

void CAN0_Handler(){
    uint8_t can_sr = CAN0->CAN_SR;

    if(can_sr & CAN_SR_MB1){
        can_receive(&m_frame, 1);
    }
    else if(can_sr & CAN_SR_MB2){
        can_receive(&m_frame, 2);
    }

    switch(m_frame.id){
        case INTERNODE_ID_RESET:
            system_reset();
            break;

        case INTERNODE_ID_SOLENOID:
            g_COMMAND.solenoid = m_frame.data[0];
            break;

        case INTERNODE_ID_POSITION:
            g_COMMAND.position_or_speed = m_frame.data[0];
            g_COMMAND.command_type = INTERNODE_COMMAND_POSITION;
            break;

        case INTERNODE_ID_SPEED:
            g_COMMAND.position_or_speed = m_frame.data[0];
            g_COMMAND.command_type = INTERNODE_COMMAND_SPEED;
            break;

        case INTERNODE_ID_SERVO:
            g_COMMAND.servo = m_frame.data[0];
            break;

        default:
            break;
    }

    NVIC_ClearPendingIRQ(ID_CAN0);
}

void internode_end_game(){
    CAN_MESSAGE end_game_frame;

    end_game_frame.id = INTERNODE_ID_END_GAME;
    end_game_frame.data_length = 0;

    can_send(&end_game_frame, 0);
}
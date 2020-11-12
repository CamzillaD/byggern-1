#include "internode.h"
#include "can_controller.h"
#include "system_reset.h"
#include "sam.h"

/* Messages this module sends */
#define INTERNODE_ID_END_GAME     0x09

/* Messages this module receives */
#define INTERNODE_ID_RESET        0x05
#define INTERNODE_ID_SOLENOID     0x18
#define INTERNODE_ID_POSITION     0x13
#define INTERNODE_ID_SPEED        0x12
#define INTERNODE_ID_SERVO        0x16

static CAN_MESSAGE m_frame;

static uint8_t m_request_reset = 0;
static InternodeCommand m_command;
static uint8_t m_speed;
static uint8_t m_servo;

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
            m_command.solenoid = m_frame.data[0];
            break;

        case INTERNODE_ID_POSITION:
            m_command.position_or_speed = m_frame.data[0];
            m_command.command_type = INTERNODE_COMMAND_POSITION;
            break;

        case INTERNODE_ID_SPEED:
            m_command.position_or_speed = m_frame.data[0];
            m_command.command_type = INTERNODE_COMMAND_SPEED;
            break;

        case INTERNODE_ID_SERVO:
            m_command.servo = m_frame.data[0];
            break;

        default:
            break;
    }

    NVIC_ClearPendingIRQ(ID_CAN0);
}

void internode_command(InternodeCommand * p_command){
    p_command->solenoid = m_command.solenoid;
    p_command->servo = m_command.servo;
    p_command->position_or_speed = m_command.position_or_speed;
    p_command->command_type = m_command.command_type;
}

#ifndef INTERNODE_H
#define INTERNODE_H
#include <stdint.h>

typedef enum {
    INTERNODE_COMMAND_SPEED,
    INTERNODE_COMMAND_POSITION
} InternodeCommandType;

typedef struct {
    uint8_t solenoid;
    uint8_t position_or_speed;
    uint8_t servo;
    InternodeCommandType command_type;
} InternodeCommand;

void internode_command(InternodeCommand * p_command);

void internode_end_game();

#endif

#include "system_reset.h"
#include <stdint.h>

/* Application Interrupt/Reset Control Register */
#define REG_AICR (*((uint32_t *)(0xe000ed0c)))
#define AICR_VECTKEY 0x05fa0000
#define AICR_RESET 0x00000001

void system_reset(){
    REG_AICR = AICR_VECTKEY | AICR_RESET;
}

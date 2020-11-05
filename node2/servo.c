#include "servo.h"


uint32_t servo_convert(double voltage){
    //possibly assert the value
    return (voltage - SERVO_VOLTAGE_MIN) * (1 / (SERVO_VOLTAGE_MAX - SERVO_VOLTAGE_MIN));
}

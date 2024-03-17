//
// Created by 17273 on 2024/3/14.
//
#include "YU_GIMBAL.h"
#include "YU_MOTOR.h"
#include "YU_UART.h"
#include "YU_DEFINE.h"

float GIMBAL_OUT[2] = {0};

void YU_F_GIMBAL_MECANUM(YU_TYPEDEF_MOTOR *MOTOR, uint32_t ID, YU_TYPEDEF_DBUS *DBUS)
{
    float REMOTE[4] = {0};
    REMOTE[0] = DBUS->REMOTE.CH0_int16;
    REMOTE[1] = DBUS->REMOTE.CH1_int16;
    REMOTE[2] = DBUS->REMOTE.CH2_int16;
    REMOTE[3] = DBUS->REMOTE.CH3_int16;

//    float

}
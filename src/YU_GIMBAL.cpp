//
// Created by 17273 on 2024/3/14.
//
#include "YU_GIMBAL.h"
#include "YU_MOTOR.h"
#include "YU_UART.h"
#include "YU_DEFINE.h"
#include "YU_PID.h"
#include "YU_CAN.h"

float GIMBAL_OUT[2] = {0};
float GIMBAL_TARGET[2] = {0};

void YU_F_GIMBAL(YU_TYPEDEF_DBUS *DBUS, int MOD)
{
    if (MOD == YU_D_MOD_CHASSIS) return;
    float REMOTE[4] = {0};
    REMOTE[0] = DBUS->REMOTE.CH0_int16;
    REMOTE[1] = DBUS->REMOTE.CH1_int16;
    REMOTE[2] = DBUS->REMOTE.CH2_int16;
    REMOTE[3] = DBUS->REMOTE.CH3_int16;

//    float
    GIMBAL_TARGET[YU_D_MOTOR_GIMBAL_YAW] += REMOTE[0] * 0.001f;
    GIMBAL_TARGET[YU_D_MOTOR_GIMBAL_PIT] += REMOTE[2] * 0.001f;

}

void YU_F_GIMBAL_CAL(YU_TYPEDEF_MOTOR *MOTOR, uint32_t ID)
{
    switch (ID)
    {
        case YU_D_MOTOR_GIMBAL_YAW:
        {
            float TARGET_ANGLE_YAW = YU_T_PID_CAL(&MOTOR->PID_A, GIMBAL_TARGET[YU_D_MOTOR_GIMBAL_YAW], MOTOR->DATA.ANGLE_NOW);

            GIMBAL_OUT[YU_D_MOTOR_GIMBAL_YAW] = YU_T_PID_CAL(&MOTOR->PID_S, TARGET_ANGLE_YAW, MOTOR->DATA.SPEED_NOW);
        }
            break;

        case YU_D_MOTOR_GIMBAL_PIT:
        {
            float TARGET_ANGLE_PIT = YU_T_PID_CAL(&MOTOR->PID_A, GIMBAL_TARGET[YU_D_MOTOR_GIMBAL_PIT], MOTOR->DATA.ANGLE_NOW);

            GIMBAL_OUT[YU_D_MOTOR_GIMBAL_PIT] = YU_T_PID_CAL(&MOTOR->PID_S, TARGET_ANGLE_PIT, MOTOR->DATA.SPEED_NOW);
        }
            break;
    }


}
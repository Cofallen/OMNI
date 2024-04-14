//
// Created by 17273 on 2024/3/14.
//
#include "YU_GIMBAL.h"
#include "YU_MOTOR.h"
#include "YU_UART.h"
#include "YU_DEFINE.h"
#include "YU_PID.h"
#include "YU_CAN.h"
#include "YU_THREAD.h"
#include "YU_MATH.h"

void YU_F_GIMBAL(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS)
{
    MOTOR[YU_D_MOTOR_GIMBAL_YAW].DATA.AIM += -(float)DBUS->REMOTE.CH2_int16 * 0.002f;
    MOTOR[YU_D_MOTOR_GIMBAL_PIT].DATA.AIM +=  (float)DBUS->REMOTE.CH3_int16 * 0.002f;

    if (MOTOR[YU_D_MOTOR_GIMBAL_PIT].DATA.AIM > 6500)
    {
        MOTOR[YU_D_MOTOR_GIMBAL_PIT].DATA.AIM = 6500;
    }
    else if (MOTOR[YU_D_MOTOR_GIMBAL_PIT].DATA.AIM < 4500)
    {
        MOTOR[YU_D_MOTOR_GIMBAL_PIT].DATA.AIM = 4500;
    }

    // PID
    MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_A.OUT.ALL_OUT = YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_A, MOTOR[YU_D_MOTOR_GIMBAL_YAW].DATA.AIM, (float)MOTOR[YU_D_MOTOR_GIMBAL_YAW].DATA.ANGLE_INFINITE);
    MOTOR[YU_D_MOTOR_GIMBAL_YAW].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_S, MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_A.OUT.ALL_OUT, MOTOR[YU_D_MOTOR_GIMBAL_YAW].DATA.SPEED_NOW);

    MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_A.OUT.ALL_OUT = YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_A, MOTOR[YU_D_MOTOR_GIMBAL_PIT].DATA.AIM, (float)MOTOR[YU_D_MOTOR_GIMBAL_PIT].DATA.ANGLE_NOW);
    MOTOR[YU_D_MOTOR_GIMBAL_PIT].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_S, MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_A.OUT.ALL_OUT, MOTOR[YU_D_MOTOR_GIMBAL_PIT].DATA.SPEED_NOW);

//    printf("OUT:  %f %f\n",GIMBAL_OUT[0],GIMBAL_OUT[1]);

//    printf("data: %hd %hd\n",YU_V_MOTOR_GIMBAL[0].DATA.ANGLE_INFINITE,YU_V_MOTOR_GIMBAL[1].DATA.ANGLE_NOW);

}

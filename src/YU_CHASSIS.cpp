//
// Created by 17273 on 2024/3/8.
//

#include "YU_CHASSIS.h"
#include "YU_PID.h"
#include "YU_UART.h"
#include "YU_MATH.h"
#include "YU_CAN.h"
#include "YU_DEFINE.h"

#include <cmath>

// 数据
Chassis_Typedef ChassisData;
Mecanum_Typedef MecanumData;
float MECANUM_TARGET[4] = {0};

/**
 * @brief 地盘功率分配
 * @param TURNFAACTOR
 * @return
 */
float *YU_F_CALCULATE_WHEELPOWERS(const float TURNFAACTOR)
{
    static float MAX_TURNFACTOR = 3640;

    static float COMPONENT[2] = {0};
    // 根据转向因子调整X和Y方向的功率分配
    COMPONENT[0] = ((MAX_TURNFACTOR - TURNFAACTOR) / MAX_TURNFACTOR);
    COMPONENT[1] = (1 - (MAX_TURNFACTOR - TURNFAACTOR) / MAX_TURNFACTOR);

    return COMPONENT;
}

/**
 * @brief 自己写的麦轮地盘函数，不知道能不能用
 * @details 小车基本物理量初始化
 * @date 2024-03-12
 * @author YU
 */
void YU_F_CHASSIS_INIT()
{
// 小车基本物理量
    MecanumData.Wheel_Perimeter = 478;
    MecanumData.Wheel_Track = 365;
    MecanumData.Wheel_Base = 305;
    MecanumData.Rotate_x_Offset = 0;
    MecanumData.Rotate_y_Offset = 0;
    MecanumData.Deceleration_ratio = 1.0f / 19.0f;
    MecanumData.Max_vx_Speed = 5000;
    MecanumData.Max_vy_Speed = 5000;
    MecanumData.Max_vr_Speed = 90;
    MecanumData.Max_Wheel_ramp = 8000;

    // 计算旋转比率
    MecanumData.Raid_FR = ((MecanumData.Wheel_Track + MecanumData.Wheel_Base) / 2.0f - MecanumData.Rotate_x_Offset + MecanumData.Rotate_y_Offset) / radian_angle;
    MecanumData.Raid_FL = ((MecanumData.Wheel_Track + MecanumData.Wheel_Base) / 2.0f - MecanumData.Rotate_x_Offset - MecanumData.Rotate_y_Offset) / radian_angle;
    MecanumData.Raid_BL = ((MecanumData.Wheel_Track + MecanumData.Wheel_Base) / 2.0f + MecanumData.Rotate_x_Offset - MecanumData.Rotate_y_Offset) / radian_angle;
    MecanumData.Raid_BR = ((MecanumData.Wheel_Track + MecanumData.Wheel_Base) / 2.0f + MecanumData.Rotate_x_Offset + MecanumData.Rotate_y_Offset) / radian_angle;

    // 将数据转化为rpm
    MecanumData.Wheel_rpm_ratio = 60.0f / (MecanumData.Wheel_Perimeter * MecanumData.Deceleration_ratio);
}


/**
 * @brief 自己写的麦轮地盘函数，不知道能不能用
 * @details 解算，将遥控数据转化成电机目标值
 * @param DBUS
 */
void YU_F_CHASSIS_MECANUM(YU_TYPEDEF_DBUS *DBUS, int MOD)
{
    if (MOD == YU_D_MOD_GIMBAL) return;
    // 将遥控器摇杆数据保存，便于使用
    float REMOTE[4] = {0};                        // 用来做限幅，不破坏遥控起原数据
    REMOTE[0] = (float)DBUS->REMOTE.CH0_int16;
    REMOTE[1] = (float)DBUS->REMOTE.CH1_int16;
    REMOTE[2] = -(float)DBUS->REMOTE.CH2_int16;
    REMOTE[3] = (float)DBUS->REMOTE.CH3_int16;    // 算追踪的， 先不写

    REMOTE[1] = YU_D_MATH_LIMIT(MecanumData.Max_vx_Speed, -MecanumData.Max_vx_Speed, REMOTE[1]);
    REMOTE[0] = YU_D_MATH_LIMIT(MecanumData.Max_vy_Speed, -MecanumData.Max_vy_Speed, REMOTE[0]);
    REMOTE[2] = YU_D_MATH_LIMIT(MecanumData.Max_vr_Speed, -MecanumData.Max_vr_Speed, REMOTE[2]);

    MecanumData.Mecanum_Out[0] = ( REMOTE[1] + REMOTE[0] - REMOTE[2] * MecanumData.Raid_FL) * MecanumData.Wheel_rpm_ratio;
    MecanumData.Mecanum_Out[1] = (-REMOTE[1] + REMOTE[0] - REMOTE[2] * MecanumData.Raid_FR) * MecanumData.Wheel_rpm_ratio;
    MecanumData.Mecanum_Out[2] = (-REMOTE[1] - REMOTE[0] - REMOTE[2] * MecanumData.Raid_BR) * MecanumData.Wheel_rpm_ratio;
    MecanumData.Mecanum_Out[3] = ( REMOTE[1] - REMOTE[0] - REMOTE[2] * MecanumData.Raid_BL) * MecanumData.Wheel_rpm_ratio;

//    printf("MOTOR1:  %f  MOTOR2:  %f  MOTOR3:  %f  MOTOR4:  %f\n",
//           MecanumData.Mecanum_Out[0],
//           MecanumData.Mecanum_Out[1],
//           MecanumData.Mecanum_Out[2],
//           MecanumData.Mecanum_Out[3]
//    );

    float TEMPMAX = 0.0f;
    for (int i = 0; i < 4; ++i)
    {
        if (YU_D_MATH_ABS(MecanumData.Mecanum_Out[i]) >= TEMPMAX)
        {
            TEMPMAX = YU_D_MATH_ABS(MecanumData.Mecanum_Out[i]);
        }
    }

    if (TEMPMAX > MecanumData.Max_Wheel_ramp)
    {
        float RATE = (float)MecanumData.Max_Wheel_ramp / TEMPMAX;
        for (int i = 0; i < 4; ++i)
        {
            MecanumData.Mecanum_Out[i] *= RATE;
        }
    }

    // 接收处理后的目标值数组
    for (int i = 0; i < 4; ++i)
    {
        MECANUM_TARGET[i] = MecanumData.Mecanum_Out[i];
    }

//    printf("TARGET:  MOTOR1:  %f  MOTOR2:  %f  MOTOR3:  %f  MOTOR4:  %f\n",
//           MECANUM_TARGET[0],
//           MECANUM_TARGET[1],
//           MECANUM_TARGET[2],
//           MECANUM_TARGET[3]
//    );

}

/**
 * @details 地盘 CAN 发送
 * @param MOTOR
 */
void YU_F_CHASSIS_MECANUM_SEND(YU_TYPEDEF_MOTOR *MOTOR)
{
    // PID 先只写一个速度环
    int16_t SPEED_SEND[4] = {0};

    SPEED_SEND[YU_D_MOTOR_CHASSIS_1] = (int16_t)YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_1].PID_S, MECANUM_TARGET[0], MOTOR->DATA.SPEED_NOW);
    SPEED_SEND[YU_D_MOTOR_CHASSIS_2] = (int16_t)YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_2].PID_S, MECANUM_TARGET[1], MOTOR->DATA.SPEED_NOW);
    SPEED_SEND[YU_D_MOTOR_CHASSIS_3] = (int16_t)YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_3].PID_S, MECANUM_TARGET[2], MOTOR->DATA.SPEED_NOW);
    SPEED_SEND[YU_D_MOTOR_CHASSIS_4] = (int16_t)YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_4].PID_S, MECANUM_TARGET[3], MOTOR->DATA.SPEED_NOW);

    printf("SMOTOR1:  %hd  SMOTOR2:  %hd\nSMOTOR3:  %hd  SMOTOR4:  %hd\n",
           SPEED_SEND[0],
           SPEED_SEND[1],
           SPEED_SEND[2],
           SPEED_SEND[3]
    );

    YU_F_CAN_SEND(0, 0x200, SPEED_SEND[YU_D_MOTOR_CHASSIS_1], SPEED_SEND[YU_D_MOTOR_CHASSIS_2], SPEED_SEND[YU_D_MOTOR_CHASSIS_3], SPEED_SEND[YU_D_MOTOR_CHASSIS_4]);

}
//
// Created by 17273 on 2024/3/8.
//

#include "YU_CHASSIS.h"
#include "YU_PID.h"
#include "YU_UART.h"
#include "YU_MATH.h"
#include "YU_CAN.h"
#include "YU_DEFINE.h"
#include "YU_THREAD.h"

#include <cmath>

// 数据
Chassis_Typedef ChassisData;
Mecanum_Typedef MecanumData;

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
    MecanumData.Max_Wheel_ramp = 2000;

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
void YU_F_CHASSIS_MECANUM(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS, int MOD)
{
    if (MOD == YU_D_MOD_GIMBAL) return;

    float VX=0.0f, VY=0.0f, VR=0.0f;
    double ANGLE_RELATIVE = 0.0f,ANGLE_RAD = 0.0f;       // 得到vx,vy,vr,angle
    VX =  (float)DBUS->REMOTE.CH1_int16;
    VY =  (float)DBUS->REMOTE.CH0_int16;
    VR = -(float)DBUS->REMOTE.CH2_int16;

    // 底盘跟随模式
    if (MOD == YU_D_MOD_CHASSIS_TRACE)
    {
        static bool STR = false;         // 排除DATA.ANGLE_LAST 一开始为0  TODO: 有点丑陋,以后不加缩进
        if (YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_YAW].DATA.ANGLE_LAST == 0) STR = true;

        if (!STR)  // STR == false
        {
            ANGLE_RELATIVE = (double)(YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_YAW].DATA.ANGLE_NOW - YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_YAW].DATA.ANGLE_LAST);
            if (ANGLE_RELATIVE > 4096.0f) ANGLE_RELATIVE -= 8192.0f;
            else if (ANGLE_RELATIVE < -4096.0f) ANGLE_RELATIVE += 8192.0f;

            ANGLE_RAD = ANGLE_RELATIVE * YU_D_RELATIVE_CAL_PARAM;
            //得到相对角度的sin，cos
            auto angleSin = std::sin(ANGLE_RAD);
            auto angleCos = std::cos(ANGLE_RAD);
            //得到转化后的vx,vy,vr
            VX = -VY * angleSin + VX * angleCos;
            VY =  VY * angleCos + VX * angleSin;
        }

    }

    VX = YU_D_MATH_LIMIT(MecanumData.Max_vx_Speed, -MecanumData.Max_vx_Speed, VX);
    VY = YU_D_MATH_LIMIT(MecanumData.Max_vy_Speed, -MecanumData.Max_vy_Speed, VY);
    VR = YU_D_MATH_LIMIT(MecanumData.Max_vr_Speed, -MecanumData.Max_vr_Speed, VR);

    MecanumData.Mecanum_Out[0] = ( VX + VY - VR * MecanumData.Raid_FL) * MecanumData.Wheel_rpm_ratio;
    MecanumData.Mecanum_Out[1] = (-VX + VY - VR * MecanumData.Raid_FR) * MecanumData.Wheel_rpm_ratio;
    MecanumData.Mecanum_Out[2] = (-VX - VY - VR * MecanumData.Raid_BR) * MecanumData.Wheel_rpm_ratio;
    MecanumData.Mecanum_Out[3] = ( VX - VY - VR * MecanumData.Raid_BL) * MecanumData.Wheel_rpm_ratio;

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
       MOTOR[i].DATA.AIM = MecanumData.Mecanum_Out[i];
    }

}

/**
 * @details 地盘 CAN 发送
 * @param MOTOR
 */
void YU_F_CHASSIS_MECANUM_CAL(YU_TYPEDEF_MOTOR *MOTOR)
{
    // PID 先只写一个速度环

    MOTOR[YU_D_MOTOR_CHASSIS_1].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_1].PID_S, MOTOR[YU_D_CAN_ID_CHASSIS_1].DATA.AIM, MOTOR->DATA.SPEED_NOW);
    MOTOR[YU_D_MOTOR_CHASSIS_2].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_2].PID_S, MOTOR[YU_D_CAN_ID_CHASSIS_2].DATA.AIM, MOTOR->DATA.SPEED_NOW);
    MOTOR[YU_D_MOTOR_CHASSIS_3].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_3].PID_S, MOTOR[YU_D_CAN_ID_CHASSIS_3].DATA.AIM, MOTOR->DATA.SPEED_NOW);
    MOTOR[YU_D_MOTOR_CHASSIS_4].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_4].PID_S, MOTOR[YU_D_CAN_ID_CHASSIS_4].DATA.AIM, MOTOR->DATA.SPEED_NOW);

}
//
// Created by 17273 on 2024/3/8.
//

#ifndef DEMO_YU_CHASSIS_H
#define DEMO_YU_CHASSIS_H

#include "YU_MOTOR.h"
#include "YU_UART.h"

// 弧度转化角度
#define radian_angle 57.3f

/**
 * @brief 地盘速度数据
 * @param Chassis_Vx x轴速度
 * @param Chassis_Vy y轴速度
 * @param Chassis_Vr 转动速度
 *
 */
typedef struct Chassis_Typedef
{
    float Chassis_Vx;
    float Chassis_Vy;
    float Chassis_Vr;
} Chassis_Typedef;

/**
 * @brief
 *
 */
typedef struct Mecanum_Typedef
{
    float Wheel_Perimeter;    // 轮的周长 mm
    float Wheel_Track;        // 轮距 mm
    float Wheel_Base;         // 轴距 mm
    float Rotate_x_Offset;    // 相对于地盘中心x轴的偏移量
    float Rotate_y_Offset;    // 相对于地盘中心y轴的偏移量
    float Deceleration_ratio; // 电机减速比
    int Max_vx_Speed;         // 地盘的x轴的最大速度
    int Max_vy_Speed;         // 地盘的y轴的最大速度
    int Max_vr_Speed;         // 地盘自转的最大速度
    int Max_Wheel_ramp;       // 限定的3508最大转速

    // 每个轮子的旋转比率，与旋转中心点相关
    float Raid_FR; // 右前
    float Raid_FL; // 左前
    float Raid_BR; // 右后
    float Raid_BL; // 左后

    float Wheel_rpm_ratio; // 将速度转化成转每分钟
    float Mecanum_Out[4];  // 分配后的输出
} Mecanum_Typedef;


float *YU_F_CALCULATE_WHEELPOWERS(const float TURNFAACTOR);

extern float MECANUM_TARGET[4];
extern struct Chassis_Typedef ChassisData;
extern struct Mecanum_Typedef MecanumData;


void YU_F_CHASSIS_INIT();
void YU_F_CHASSIS_MECANUM(YU_TYPEDEF_DBUS *DBUS, int MOD);
void YU_F_CHASSIS_MECANUM_SEND(YU_TYPEDEF_MOTOR *MOTOR);

#endif //DEMO_YU_CHASSIS_H

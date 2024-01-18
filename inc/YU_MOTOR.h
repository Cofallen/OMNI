//
// Created by 17273 on 2024/1/15.
//

#ifndef DEMO_YU_MOTOR_H
#define DEMO_YU_MOTOR_H

#include <cstdint>

//按照RUI的代码写的电机数据结构体打印板
struct YU_MOTOR_DATA_TYPEDEF
{
    int8_t ONLINE_JUDGE_TIME; //
    int16_t ANGLE_LAST;
    int16_t ANGLE_NOW;
    int16_t SPEED_LAST;
    int16_t SPEED_NOW;
    int16_t CURRENT;
    int32_t ANGLE_INFINITE;
    int16_t ANGLE_INIT;
    int16_t ANGLE_RELATIVE; // 相对角度
    uint16_t STUCK_TIME;
    int16_t LAPS;
    float AIM;
    float AIM_LAST;

    int16_t TEMP;
};

// PID参数模板
struct YU_MOTOR_PID_TYPEDEF
{
    struct
    {
        float KP;
        float KI;
        float KD;
        float I_LIT;
        float ALL_LIT;
    }IN;
    struct
    {
        float ERROR[2];
        float P_OUT;
        float I_OUT;
        float D_OUT;
        float ALL_OUT;
    }OUT;
};

typedef struct
{
    uint8_t PID_INIT;
    struct YU_MOTOR_DATA_TYPEDEF DATA; // 电机数据
    struct YU_MOTOR_PID_TYPEDEF PID_A; // 角度环
    struct YU_MOTOR_PID_TYPEDEF PID_S; // 速度环
    struct YU_MOTOR_PID_TYPEDEF PID_C; // 电流环
} YU_TYPEDEF_MOTOR;

#endif //DEMO_YU_MOTOR_H

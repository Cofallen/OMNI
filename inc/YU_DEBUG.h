//
// Created by 17273 on 2024/1/15.
//
#ifdef YU_D_DEBUG
#ifndef DEMO_YU_DEBUG_HS
#define DEMO_YU_DEBUG_H

#include "YU_MOTOR.h"
#include "YU_DEFINE.h"

struct YU_TYPEDEF_MOTOR_DATA_DEBUG
{
    float ANGLE;
    float SPEED;
    float CURRENT;
    float ANGLE_INFINITE;
    float LAPS;
    float AIM;
};

struct YU_TYPEDEF_DEBUG
{
    struct YU_TYPEDEF_MOTOR_DATA_DEBUG DATA;
    struct YU_TYPEDEF_MOTOR_PID PID_A;
    struct YU_TYPEDEF_MOTOR_PID PID_S;
    struct YU_TYPEDEF_MOTOR_PID PID_C;  //电流环
};

extern struct YU_TYPEDEF_DEBUG YU_V_DEBUG[10];


void YU_F_DEBUG_THREAD();


#endif //DEMO_YU_DEBUG_H

#endif
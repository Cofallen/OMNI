//
// Created by 17273 on 2024/1/15.
//

#ifndef DEMO_YU_DEBUG_H
#define DEMO_YU_DEBUG_H

#include "YU_MOTOR.h"
#include "YU_DEFINE.h"

struct YU_MOTOR_DATA_DEBUG_TYPEDEF
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
    struct YU_MOTOR_DATA_DEBUG_TYPEDEF DATA;
    struct YU_MOTOR_DATA_DEBUG_TYPEDEF PID_A;
    struct YU_MOTOR_DATA_DEBUG_TYPEDEF PID_S;
    struct YU_MOTOR_DATA_DEBUG_TYPEDEF PID_C;  //电流环
};
extern struct YU_TYPEDEF_DEBUG YU_V_DEBUG[10];

#endif //DEMO_YU_DEBUG_H

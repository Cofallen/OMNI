//
// Created by 17273 on 2024/3/12.
//

#ifndef DEMO_YU_THREAD_H
#define DEMO_YU_THREAD_H

#include "YU_UART.h"
#include "YU_MOTOR.h"
#include "YU_TOP.h"

#include <iostream>

// VOFA 数据包
typedef struct YU_TYPEDEF_DEBUG
{
    struct
    {
        float ANGLE;
        float SPEED;
        float CURRENT;
        float ANGLE_INFINITE;
        float LAPS;
        float AIM;
    }MOTOR_DATA;
//    struct
//    {
//        float MECANUM[4];
//        float GIMBAL[2];
//    }TARGET;
    YU_TYPEDEF_MOTOR_PID PID_A;
    YU_TYPEDEF_MOTOR_PID PID_S;
    YU_TYPEDEF_MOTOR_PID PID_C;
}YU_TYPEDEF_DEBUG_;

typedef union
{
    struct
    {
        YU_TYPEDEF_DEBUG YU_V_MOTOR_DEBUG;
        char TAIL[4];
    } DATA;
    char ALL[160];
}YU_TYPEDEF_SEND_UNION;
typedef union
{
    struct
    {
        char NAME[12];
        float PARAM;
    } DATA;
    char ALL[16];
}YU_TYPEDEF_RECV_UNION;


extern YU_TYPEDEF_DBUS YU_V_DBUS;
extern YU_TYPEDEF_MONITOR_DBUS YU_V_MONITOR_DBUS;

extern YU_TYPEDEF_MOTOR YU_V_MOTOR_CHASSIS[4];
extern YU_TYPEDEF_TOP YU_V_TOP_DATA_CHASSIS;
extern YU_TYPEDEF_MOTOR YU_V_MOTOR_GIMBAL[2];
extern YU_TYPEDEF_TOP YU_V_TOP_DATA_GIMBAL;

extern YU_TYPEDEF_DEBUG YU_V_DEBUG[10];
extern int8_t MOTOR_TYPE;

[[noreturn]] void YU_F_THREAD_CHASSIS_MECANUM();
[[noreturn]] void YU_F_THREAD_TEST();
[[noreturn]] void YU_F_THREAD_MONITOR();
[[noreturn]] void YU_F_THREAD_GIMBAL();
[[noreturn]] void YU_F_THREAD_VOFA();
[[noreturn]] void YU_F_THREAD_EXIT(int signum);




#endif //DEMO_YU_THREAD_H

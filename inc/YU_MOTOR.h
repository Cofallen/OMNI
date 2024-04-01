//
// Created by 17273 on 2024/1/15.
//

#ifndef DEMO_YU_MOTOR_H
#define DEMO_YU_MOTOR_H

#include <cstdint>

//按照RUI的代码写的电机数据结构体打印板
struct YU_TYPEDEF_MOTOR_DATA
{
    int8_t ONLINE_JUDGE_TIME; //
    int16_t ANGLE_LAST;
    int16_t ANGLE_NOW;
    int16_t SPEED_LAST;
    int16_t SPEED_NOW;
    int16_t CURRENT;
    int32_t ANGLE_INFINITE;

    uint16_t STUCK_TIME;
    int16_t LAPS;
    float AIM;
    float AIM_LAST;

    int16_t CAN_SEND;     //CAN发送数据

    int16_t TEMP;

    int ANGLE_INIT;       //云台初始角度
    int ANGLE_MAX;        //云台最大角度
    int ANGLE_MIN;        //云台最小角度
};

// PID参数模板
struct YU_TYPEDEF_MOTOR_PID
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
    struct YU_TYPEDEF_MOTOR_DATA DATA; // 电机数据
    struct YU_TYPEDEF_MOTOR_PID PID_A; // 角度环
    struct YU_TYPEDEF_MOTOR_PID PID_S; // 速度环
    struct YU_TYPEDEF_MOTOR_PID PID_C; // 电流环
} YU_TYPEDEF_MOTOR;


void YU_F_MOTOR_CAN_CAL(YU_TYPEDEF_MOTOR *MOTOR,const uint8_t *CAN_DATA,uint8_t TYPE,uint8_t STATUS_ID);
void YU_F_MOTOR_CLEAR(YU_TYPEDEF_MOTOR *MOTOR, uint8_t *TYPE);


#endif //DEMO_YU_MOTOR_H

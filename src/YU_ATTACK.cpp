//
// Created by 17273 on 2024/4/1.
//
#include <string>
#include "YU_ATTACK.h"
#include "YU_MOTOR.h"
#include "YU_DEFINE.h"
#include "YU_CONFIG.h"
#include "YU_UART.h"

struct
{
    int test;
} YU_TYPEDEF_ATTACK;

bool YU_F_ATTACK_INIT()
{

    printf("ATTACK init OK\n");
    return true;
}

float YU_F_ATTACK_FIRE_AIM(YU_TYPEDEF_DBUS *DBUS, YU_TYPEDEF_MOTOR *MOTOR, uint8_t VISION)
{

    return 0.0f;
}

void YU_F_ATTACK_CHECK(){}

bool YU_F_ATTACK(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS, int8_t VISION_TYPE)
{
    static bool STR = false;
    if (!STR)
    {
        YU_F_ATTACK_INIT();
    }


    return true;
}

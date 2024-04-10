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
    int WIPE_MAX_SPEED;
    int WIPE_MIN_SPEED;
    int WIPE_MID_SPEED;
    int SINGLE_ANGLE;
} YU_TYPEDEF_ATTACK;

bool YU_F_ATTACK_INIT()
{
    std::string FILENAME = YU_D_CONFIG_PATH;
    YU_TYPEDEF_PID_CONFIG CONFIG = YU_F_PARSE_CONFIG(FILENAME);
    YU_TYPEDEF_ATTACK.WIPE_MAX_SPEED = std::stoi(CONFIG[ "ATTACK" ][ "MAX_SPEED" ]);
    YU_TYPEDEF_ATTACK.WIPE_MIN_SPEED = std::stoi(CONFIG[ "ATTACK" ][ "MIN_SPEED" ]);
    YU_TYPEDEF_ATTACK.WIPE_MID_SPEED = std::stoi(CONFIG[ "ATTACK" ][ "MID_SPEED" ]);
    YU_TYPEDEF_ATTACK.SINGLE_ANGLE = std::stoi(CONFIG[ "ATTACK" ][ "SINGLE_ANGLE" ]);
    printf("ATTACK init OK\n");
    return true;
}

float YU_F_GET_FIRE_WIPE_SPEED(YU_TYPEDEF_DBUS *DBUS, YU_TYPEDEF_MOTOR *MOTOR, uint8_t VISION)
{
    static uint8_t SINGLE_LOCK = 0;
    static int64_t AIM = 0;
    uint8_t MOD = DBUS->R_FLAG;
    // 停止
    if (MOD == 0)
    {
        // 单发解锁
        SINGLE_LOCK = YU_D_LOCK_ON;
        AIM = MOTOR->DATA.ANGLE_INFINITE;
    } else
    {
        SINGLE_LOCK = YU_D_LOCK_OFF;
    }
    return 0.0f;
}

bool YU_F_ATTACK(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS, int8_t VISION_TYPE)
{
    static bool STR = false;
    if (!STR)
    {
        YU_F_ATTACK_INIT();
    }

    MOTOR[YU_D_MOTOR_ATTACK_L].DATA.AIM = (float)YU_TYPEDEF_ATTACK.WIPE_MAX_SPEED;

    return true;
}
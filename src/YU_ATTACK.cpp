//
// Created by 17273 on 2024/4/1.
//
#include <string>
#include "YU_ATTACK.h"
#include "YU_MOTOR.h"
#include "YU_DEFINE.h"
#include "YU_CONFIG.h"
#include "YU_UART.h"
#include "YU_MATH.h"

struct
{
    int16_t G_CURRENT_MAX;
    int16_t G_ANGLE[2];
    int16_t G_ANGLE_ERROR;
    struct timeval G_TIMEOUT_MAX;
} YU_V_MONITOR_ATTACK{ };

bool YU_F_ATTACK_INIT()
{

    printf("ATTACK init OK\n");
    return true;
}

float YU_F_ATTACK_FIRE_AIM(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS, uint8_t VISION)
{

    return 0.0f;
}

void YU_F_ATTACK_CHECK(YU_TYPEDEF_MOTOR *MOTOR)
{
    // 我想根据电流异常，超时时间，角度变化来确定卡弹，待测试
    YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_LAST] = YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW];
    YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW]  = MOTOR->DATA.ANGLE_NOW;

    if (MOTOR->DATA.CURRENT >= YU_V_MONITOR_ATTACK.G_CURRENT_MAX)
    {
        // 异常处理
    }

    YU_V_MONITOR_ATTACK.G_ANGLE_ERROR = YU_D_MATH_ABS((YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW] - YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_LAST]));

    // 超时设置
    YU_V_MONITOR_ATTACK.G_TIMEOUT_MAX.tv_usec = 10;

    usleep(YU_V_MONITOR_ATTACK.G_TIMEOUT_MAX.tv_usec);
    YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_LAST] = YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW];

    if (YU_V_MONITOR_ATTACK.G_ANGLE_ERROR < 600)
    {
        // 异常处理 倒转
    }

}

bool YU_F_ATTACK(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS, int8_t VISION_TYPE)
{

    return true;
}

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
#include "YU_CAN.h"
#include "YU_PID.h"

#define YU_D_DIRECTION_PLUS 1      // positive
#define YU_D_DIRECTION_MINUS 0     // negative

struct
{
    int16_t G_CURRENT_MAX;
    int32_t G_ANGLE[2];
    int32_t G_ANGLE_ERROR;
    struct timeval G_TIMEOUT_MAX;
    int8_t G_DIRECTION;
    int8_t FLAG;

    float SINGLE_ANGLE;          // 单发角度
    float SPEED;                 // 摩擦轮速度
} YU_V_MONITOR_ATTACK{ };

YU_TYPEDEF_MOTOR YU_V_MOTOR_ATTACK[3]{ };

bool YU_F_ATTACK_INIT()
{
    // 数据初始化
    YU_V_MONITOR_ATTACK.SINGLE_ANGLE = -36864.0f;
    YU_V_MONITOR_ATTACK.SPEED = 3000.0f;
    // 拨弹双环pid
    const float PID_ATTACK_G_A[5] = {5,0,0,0,0};
    const float PID_ATTACK_G_S[5] = {5,0,0,0,0};
    YU_T_PID_INIT(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].PID_A, PID_ATTACK_G_A);
    YU_T_PID_INIT(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].PID_S, PID_ATTACK_G_S);

    printf("ATTACK init OK\n");
    return true;
}

// 通过裁判系统弹速计算目标值
float YU_F_ATTACK_FIRE_AIM(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS, uint8_t VISION)
{
//    MOTOR->DATA.AIM =
    return 0.0f;
}

void YU_F_ATTACK_JAM(YU_TYPEDEF_MOTOR *MOTOR)
{
    // 我想根据电流异常，超时时间，角度变化来确定卡弹，待测试     还是用flag来判断方向变化吧
    YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_LAST] = YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW];
    YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW]  = MOTOR->DATA.ANGLE_INFINITE;

    if (MOTOR->DATA.CURRENT >= YU_V_MONITOR_ATTACK.G_CURRENT_MAX)
    {
        // 异常处理
    }

    YU_V_MONITOR_ATTACK.G_ANGLE_ERROR = YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW] - YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_LAST];

    // 超时设置
    YU_V_MONITOR_ATTACK.G_TIMEOUT_MAX.tv_usec = 10;


    usleep(YU_V_MONITOR_ATTACK.G_TIMEOUT_MAX.tv_usec);  // 等待拨弹稳定，最大延时时间，这里要整一个监视，不能一直延时，若监视到自动运行

    YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_LAST] = YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW];

    if (YU_V_MONITOR_ATTACK.G_ANGLE_ERROR < 200)
    {
        // 异常处理 倒转
        YU_V_MONITOR_ATTACK.FLAG = YU_D_DIRECTION_PLUS;
    } else if (YU_V_MONITOR_ATTACK.G_ANGLE_ERROR > -200)
    {
        YU_V_MONITOR_ATTACK.FLAG = YU_D_DIRECTION_MINUS;
    }



}

bool YU_F_ATTACK(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS, int8_t VISION_TYPE)
{
    // 摩擦轮发射
    YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.AIM = -YU_V_MONITOR_ATTACK.SPEED;
    YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.AIM =  YU_V_MONITOR_ATTACK.SPEED;
    return true;
}

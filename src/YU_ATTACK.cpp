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
#include "YU_THREAD.h"

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

// 通过裁判系统弹计速算摩擦轮目标值
float YU_F_ATTACK_FIRE_AIM(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS, uint8_t VISION)
{
//    MOTOR->DATA.AIM =
    return 0.0f;
}

// 判断是否卡弹，获取拨弹目标值，要判断是否是连发 没写完
float YU_F_ATTACK_JAM(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS)
{
    // 根据 超时时间，角度变化来确定卡弹，待测试     还是用flag来判断方向变化吧
    YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_LAST] = YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW];
    YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW]  = MOTOR->DATA.ANGLE_INFINITE;

    YU_V_MONITOR_ATTACK.G_ANGLE_ERROR = YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW] - YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_LAST];

    // 超时设置
    auto TIME_START = std::chrono::steady_clock::now();

    YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_LAST] = YU_V_MONITOR_ATTACK.G_ANGLE[YU_D_NOW];

    if (YU_V_MONITOR_ATTACK.G_ANGLE_ERROR < 200)
    {
        // 异常处理 倒转
        YU_V_MONITOR_ATTACK.FLAG = YU_D_DIRECTION_PLUS;
    } else if (YU_V_MONITOR_ATTACK.G_ANGLE_ERROR > -200)
    {
        YU_V_MONITOR_ATTACK.FLAG = YU_D_DIRECTION_MINUS;
    }

    // 需要计算卡弹后到底转多少度，还是积累着来，最后搞一次

    // 假设已经判断好是否卡弹，并且已算好目标值
    // 想了想，继续分三种
    int TEMP = 0;
    int COUNT = 10;
    if (DBUS->REMOTE.S2_u8 == YU_D_MOD_SINGLE && TEMP == 0)
    {
        TEMP = 1;
        if (YU_V_MONITOR_ATTACK.FLAG == YU_D_DIRECTION_PLUS) return YU_V_MONITOR_ATTACK.SINGLE_ANGLE;
        else if (YU_V_MONITOR_ATTACK.FLAG == YU_D_DIRECTION_MINUS) return YU_V_MONITOR_ATTACK.SINGLE_ANGLE;
    }
    else if (DBUS->REMOTE.S2_u8 == YU_D_MOD_CONSIST && TEMP == 0)
    {
        // 一直发，保持稳定频率 // 没有卡弹监测
        TEMP = 1;
        return YU_V_MONITOR_ATTACK.SINGLE_ANGLE * (float)COUNT;
    }
    else if (DBUS->REMOTE.S2_u8 == YU_D_MOD_SHUT)
    {
        TEMP = 0;
        return 0.0f;
    }
    return YU_V_MONITOR_ATTACK.SINGLE_ANGLE;
}

// 先把发射当成一个线程来写，后面考虑合并到云台上
[[noreturn]] void YU_F_ATTACK(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS, int8_t VISION_TYPE)
{
    static bool INIT= false;
    if (!INIT)
    {
        YU_F_ATTACK_INIT();
    }

    // 摩擦轮发射目标值
    YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.AIM = -YU_V_MONITOR_ATTACK.SPEED;
    YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.AIM =  YU_V_MONITOR_ATTACK.SPEED;
    YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = YU_F_ATTACK_JAM(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G],DBUS);

    int TEMP = 0;  // 用作辅助判断,实现单发一次
    while (true)
    {
        usleep(1);
        YU_F_CAN_RECV(YU_V_MOTOR_ATTACK, &YU_V_TOP_DATA ,YU_D_CAN_2);

        // 单发,连发,不发 控制 左拨杆
        if (DBUS->REMOTE.S2_u8 == YU_D_MOD_SINGLE && TEMP == 0) // 单发
        {
            YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = YU_F_ATTACK_JAM(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G],&YU_V_DBUS);
            TEMP = 1;
        } else if (DBUS->REMOTE.S2_u8 == YU_D_MOD_CONSIST && TEMP == 0) //连发  没写完
        {
            YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = YU_F_ATTACK_JAM(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G],&YU_V_DBUS);
            TEMP = 1;
        } else if (DBUS->REMOTE.S2_u8 == YU_D_MOD_SHUT) // 停止
        {
            YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.AIM = 0;
            YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.AIM = 0;
            YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = 0;
            TEMP = 0;
        }

        // pid 计算
        // 单环发射
        YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].PID_S,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.AIM,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.SPEED_NOW);
        YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].PID_S,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.AIM,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.SPEED_NOW);
        // 双环拨弹
        YU_T_PID_CAL(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].PID_A,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM,(float)YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE);
        YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].PID_S,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].PID_A.OUT.ALL_OUT,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.SPEED_NOW);

        YU_F_CAN_SEND(YU_D_CAN_2,0x1FF,1000,0,1000,0);
    }


}

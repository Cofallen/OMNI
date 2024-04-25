//
// Created by 17273 on 2024/4/1.
//
#include "YU_ATTACK.h"
#include "YU_MOTOR.h"
#include "YU_DEFINE.h"
#include "YU_UART.h"
#include "YU_MATH.h"
#include "YU_CAN.h"
#include "YU_PID.h"
#include "YU_THREAD.h"


struct
{
    int16_t G_CURRENT_MAX;
    int32_t G_ANGLE[2];
    int32_t G_ANGLE_ERROR;
    int32_t TIME;
    int8_t G_DIRECTION;
    int8_t FLAG;

    float SINGLE_ANGLE;          // 单发角度
    float SPEED;                 // 摩擦轮速度

    int COUNT;
} YU_V_MONITOR_ATTACK{ };

YU_TYPEDEF_MOTOR YU_V_MOTOR_ATTACK[3]{ };

bool YU_F_ATTACK_INIT()
{
    YU_V_MONITOR_ATTACK.FLAG = 1;  // 默认卡弹后取负反转
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

// 获取拨弹目标值  // 需要上锁防止单发状态下目标值疯长
float YU_F_ATTACK_JAM_AIM(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS)
{
    int8_t LOCK = 0;
    if (DBUS->REMOTE.S2_u8 == YU_D_MOD_CONSIST && LOCK == 0)
    {
//        YU_V_MONITOR_ATTACK.COUNT ++;             // 应是根据裁判系统热量获取目标值
        YU_V_MONITOR_ATTACK.COUNT = 10;             // 先写10连发
        LOCK = 1;
    } else if (DBUS->REMOTE.S2_u8 == YU_D_MOD_SINGLE && LOCK == 0)
    {
        YU_V_MONITOR_ATTACK.COUNT = 1;
        LOCK = 1;
    } else if (DBUS->REMOTE.S2_u8 == YU_D_MOD_SHUT)
    {
        YU_V_MONITOR_ATTACK.COUNT = 0;
        LOCK = 0;
    }

    MOTOR[YU_D_MOTOR_ATTACK_G].DATA.AIM = (float)MOTOR[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE + YU_V_MONITOR_ATTACK.SINGLE_ANGLE * (float)YU_V_MONITOR_ATTACK.COUNT;
    return MOTOR[YU_D_MOTOR_ATTACK_G].DATA.AIM;
}

// 判断是否卡弹，获取拨弹目标值，要判断是否是连发 没写完
/**
 * @brief 写成了若卡弹就反转，这时若不卡了，还是原方向转
 * @param MOTOR 一定是发射的数组名
 * @param DBUS
 * @return
 */
bool YU_F_ATTACK_JAM_CHECK(YU_TYPEDEF_MOTOR *MOTOR)
{
    // 根据 超时时间，角度变化来确定卡弹，待测试
    // 超时设置应该只有一个时间变量，不能是差值
    auto TIME_START = std::chrono::steady_clock::now();
    auto TIME_END   = std::chrono::steady_clock::now();
    // 若卡弹，直接回到那个稳定角度，还卡，再回相反位置
    float TEMP = MOTOR[YU_D_MOTOR_ATTACK_G].DATA.AIM - (float)MOTOR[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE;
    float EDGE = YU_V_MONITOR_ATTACK.SINGLE_ANGLE / 100.0f;                   // 这个比例系数是认为差多少比例到目标值
    if ((YU_D_MATH_ABS(TEMP)) >= EDGE)  // 是否到达边缘监测
    {
        if ((TIME_END - TIME_START) > std::chrono::milliseconds(200))
        {
            // 认为卡弹
//            YU_V_MONITOR_ATTACK.FLAG = (int8_t)(-YU_V_MONITOR_ATTACK.FLAG);
//            YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = (float)YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE + YU_V_MONITOR_ATTACK.SINGLE_ANGLE * static_cast<float>(YU_V_MONITOR_ATTACK.FLAG);
            return false;
        }else
        {
            // 认为没到边缘，但是在允许时间内
            return true;
        }
    }
    else
    {
        // 认为转到对应位置
        return true;
    }
}

// 先把发射当成一个线程来写，后面考虑合并到云台上  // 所以函数内不需传参 // 记得与云台合并时加入传参
[[noreturn]] void YU_F_THREAD_ATTACK()
{
    static bool INIT= false;
    if (!INIT)
    {
        YU_F_ATTACK_INIT();
    }

    // 摩擦轮发射目标值
    YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.AIM = -YU_V_MONITOR_ATTACK.SPEED;
    YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.AIM =  YU_V_MONITOR_ATTACK.SPEED;
    YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM =  (float)YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE;

    int TEMP = 0;  // 用作辅助判断,实现单发一次
    while (true)
    {
        usleep(1);
        YU_F_CAN_RECV(YU_V_MOTOR_ATTACK, &YU_V_TOP_DATA ,YU_D_CAN_2);

        if (!YU_F_ATTACK_JAM_CHECK(YU_V_MOTOR_ATTACK))
        {
            // 卡弹
            YU_V_MONITOR_ATTACK.FLAG = (int8_t)(-YU_V_MONITOR_ATTACK.FLAG);
            YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = (float)YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE + YU_V_MONITOR_ATTACK.SINGLE_ANGLE * static_cast<float>(YU_V_MONITOR_ATTACK.FLAG);
        }else
        {
            // 单发,连发,不发 控制 左拨杆
            if (YU_V_DBUS.REMOTE.S2_u8 == YU_D_MOD_SINGLE && TEMP == 0) // 单发
            {
                YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = YU_F_ATTACK_JAM_AIM(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G],&YU_V_DBUS);
                TEMP = 1;
            } else if (YU_V_DBUS.REMOTE.S2_u8 == YU_D_MOD_CONSIST && TEMP == 0) //连发  没写完
            {
                YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = YU_F_ATTACK_JAM_AIM(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G],&YU_V_DBUS);
                TEMP = 1;
            } else if (YU_V_DBUS.REMOTE.S2_u8 == YU_D_MOD_SHUT) // 停止
            {
                YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.AIM = 0;
                YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.AIM = 0;
                YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = 0;
                TEMP = 0;
            }
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

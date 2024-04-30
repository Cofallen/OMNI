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

// 超时时间应与拨弹量有关
#define YU_D_TIMEOUT 3000
#define YU_D_SPEEDOUT 80
struct
{
    int32_t TIME;
    int8_t FLAG;
    float SINGLE_ANGLE;          // 单发角度
    float SPEED;                 // 摩擦轮速度
    int COUNT;
} YU_V_ATTACK_PARAM{ };
int8_t LOCK = 0;

YU_TYPEDEF_MOTOR YU_V_MOTOR_ATTACK[3]{ };

bool YU_F_ATTACK_INIT()
{
    YU_V_ATTACK_PARAM.TIME = 0;
    YU_V_ATTACK_PARAM.FLAG = 0;
    // 数据初始化
    YU_V_ATTACK_PARAM.SINGLE_ANGLE = 36864.0f;
    YU_V_ATTACK_PARAM.SPEED = 3000.0f;
    // 拨弹双环pid
    const float PID_ATTACK_G_A[5] = {0.08f,0.0001f,0.4f,103,4352};
    const float PID_ATTACK_G_S[5] = {1.0f,0,2.4f,500.0f,7892};
    YU_T_PID_INIT(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].PID_A, PID_ATTACK_G_A);
    YU_T_PID_INIT(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].PID_S, PID_ATTACK_G_S);

    const float PID_ATTACK_L[5] = {5,0,0,1000,5000};
    const float PID_ATTACK_R[5] = {5,0,0,1000,5000};
    YU_T_PID_INIT(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].PID_S, PID_ATTACK_L);
    YU_T_PID_INIT(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].PID_S, PID_ATTACK_R);

    printf("ATTACK init OK\n");
    return true;
}

// 获取拨弹目标值  // 需要上锁防止单发状态下目标值疯长
float YU_F_ATTACK_JAM_AIM(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS)
{

    if (DBUS->REMOTE.S2_u8 == YU_D_MOD_SHUT)
    {
        LOCK = 0;
        YU_V_ATTACK_PARAM.COUNT = 0;    // 防止多次调用，使目标值快速增长
    }
    if (YU_V_ATTACK_PARAM.FLAG == 1)
        return MOTOR[YU_D_MOTOR_ATTACK_G].DATA.AIM;
    if (LOCK == 0)
    {
        if (DBUS->REMOTE.S2_u8 == YU_D_MOD_CONSIST)
        {
//        YU_V_MONITOR_ATTACK.COUNT ++;             // 应是根据裁判系统热量获取目标值
            YU_V_ATTACK_PARAM.COUNT = 1;             // 先写10连发
//            LOCK = 1;
        } else if (DBUS->REMOTE.S2_u8 == YU_D_MOD_SINGLE)
        {
            YU_V_ATTACK_PARAM.COUNT = 1;
            LOCK = 1;
        }
//        YU_V_ATTACK_PARAM.TIME = 0;     // 标记，忘了为什么加这个，删了

    } else if (LOCK == 1)
    {
        // 不调用aim，使aim保持第一个信号时目标值
        return MOTOR[YU_D_MOTOR_ATTACK_G].DATA.AIM;
    }

    MOTOR[YU_D_MOTOR_ATTACK_G].DATA.AIM = (float)MOTOR[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE + YU_V_ATTACK_PARAM.SINGLE_ANGLE * (float)YU_V_ATTACK_PARAM.COUNT;

    return MOTOR[YU_D_MOTOR_ATTACK_G].DATA.AIM;
}

// 判断是否卡弹
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

    // 若卡弹，直接回到那个稳定角度，还卡，再回相反位置
    float TEMP = MOTOR[YU_D_MOTOR_ATTACK_G].DATA.AIM - (float)MOTOR[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE;
    float EDGE = YU_D_MATH_ABS((YU_V_ATTACK_PARAM.SINGLE_ANGLE / 100.0f));                   // 这个比例系数是认为差多少比例到目标值
    if ((YU_D_MATH_ABS(TEMP)) <= EDGE)
    {
        YU_V_ATTACK_PARAM.FLAG = 0;
    }
    if (((YU_D_MATH_ABS(TEMP)) >= EDGE) && ((YU_D_MATH_ABS(MOTOR[YU_D_MOTOR_ATTACK_G].DATA.SPEED_NOW)) <= YU_D_SPEEDOUT))  // 是否到达边缘监测
    {

        if (YU_V_ATTACK_PARAM.TIME >= YU_D_TIMEOUT)    // 不知道循环时间，随便给的，待测试
        {
            // 认为卡弹
//            YU_V_MONITOR_ATTACK.FLAG = (int8_t)(-YU_V_MONITOR_ATTACK.FLAG);
//            YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = (float)YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE + YU_V_MONITOR_ATTACK.SINGLE_ANGLE * static_cast<float>(YU_V_MONITOR_ATTACK.FLAG);
            return false;
        }else
        {
            // 认为没到边缘，但是在允许时间内
            YU_V_ATTACK_PARAM.TIME++;
            return true;
        }
    }
    else if (YU_V_ATTACK_PARAM.FLAG == 0)
    {
        // 认为转到对应位置
        YU_V_ATTACK_PARAM.TIME = 0;
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
    YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.AIM = -YU_V_ATTACK_PARAM.SPEED;
    YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.AIM =  YU_V_ATTACK_PARAM.SPEED;
    YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM =  (float)YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE;  // 先不转

    while (true)
    {
        usleep(1);
        YU_F_CAN_RECV(YU_V_MOTOR_ATTACK, &YU_V_TOP_DATA ,YU_D_CAN_2);

        if (!YU_F_ATTACK_JAM_CHECK(YU_V_MOTOR_ATTACK))
        {
            // 卡弹
            YU_V_ATTACK_PARAM.FLAG = 1;

            YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = (float)YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE - YU_V_ATTACK_PARAM.SINGLE_ANGLE;
            if (YU_V_DBUS.REMOTE.S2_u8 == YU_D_MOD_SHUT)  // 强行终止，归0
            {
                YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = (float)YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE;
            }

            YU_V_ATTACK_PARAM.TIME = 0;  // 卡弹后时间归0，等待下次是否卡弹
            YU_V_ATTACK_PARAM.COUNT = 0;
        }else
        {
            // 根据遥控模式获取当前拨弹电机目标值  // 思考下，连续拨弹中速度不稳定问题 // 这个直接条pid 速度环给稳了估计就行
            YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM = YU_F_ATTACK_JAM_AIM(YU_V_MOTOR_ATTACK,&YU_V_DBUS);
        }

        // pid 计算
        // 单环发射
        YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].PID_S,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.AIM,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.SPEED_NOW);
        YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].PID_S,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.AIM,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.SPEED_NOW);
        // 双环拨弹
        YU_T_PID_CAL(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].PID_A,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM,(float)YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE);
        YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.CAN_SEND = (int16_t)YU_T_PID_CAL(&YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].PID_S,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].PID_A.OUT.ALL_OUT,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.SPEED_NOW);

        YU_F_CAN_SEND(YU_D_CAN_2,0x200,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_L].DATA.CAN_SEND,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_R].DATA.CAN_SEND,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.CAN_SEND,0);
//        printf("%hd\n",YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.CAN_SEND);
//        printf("%hd\n",YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.SPEED_NOW);
//        printf("ANGLE:  %d  MOD:  %d   TIME:  %d  FLAG: %d AIM  %.2f\n",YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.ANGLE_INFINITE,YU_V_DBUS.REMOTE.S2_u8,YU_V_ATTACK_PARAM.TIME,YU_V_ATTACK_PARAM.FLAG,YU_V_MOTOR_ATTACK[YU_D_MOTOR_ATTACK_G].DATA.AIM);
    }


}

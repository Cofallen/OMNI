//
// Created by 17273 on 2024/2/27.
//
#include "YU_PID.h"
#include "YU_CONFIG.h"
#include "YU_MOTOR.h"
#include "YU_DEFINE.h"
#include "YU_MATH.h"

/**
 * 角度速度双环PID
 * @param MOTOR
 */
//void YU_F_MOTOR_PID_AS(YU_TYPEDEF_MOTOR *MOTOR)
//{
//    // 角度环
//    MOTOR->PID_A.OUT.ERROR[YU_D_NOW] = MOTOR->DATA.AIM - (float)MOTOR->DATA.ANGLE_INFINITE;
//    MOTOR->PID_A.OUT.P_OUT = MOTOR->PID_A.IN.KP * MOTOR->PID_A.OUT.ERROR[YU_D_NOW];
//    MOTOR->PID_A.OUT.I_OUT += (MOTOR->PID_A.IN.KI * MOTOR->PID_A.OUT.ERROR[YU_D_NOW]);
//    MOTOR->PID_A.OUT.D_OUT = MOTOR->PID_A.IN.KD * (MOTOR->PID_A.OUT.ERROR[YU_D_NOW] - MOTOR->PID_A.OUT.ERROR[YU_D_LAST]);
//    MOTOR->PID_A.OUT.I_OUT = YU_D_MATH_LIMIT(MOTOR->PID_A.IN.I_LIT, -MOTOR->PID_A.IN.I_LIT, MOTOR->PID_A.OUT.I_OUT);
//    MOTOR->PID_A.OUT.ALL_OUT = MOTOR->PID_A.OUT.P_OUT + MOTOR->PID_A.OUT.I_OUT + MOTOR->PID_A.OUT.D_OUT;
//    MOTOR->PID_A.OUT.ALL_OUT = YU_D_MATH_LIMIT(MOTOR->PID_A.IN.ALL_LIT, -MOTOR->PID_A.IN.ALL_LIT, MOTOR->PID_A.OUT.ALL_OUT);
//
//    // 数据迭代
//    MOTOR->PID_A.OUT.ERROR[YU_D_LAST] = MOTOR->PID_A.OUT.ERROR[YU_D_NOW];
//
//    // 速度环
//    MOTOR->PID_S.OUT.ERROR[YU_D_NOW] = MOTOR->PID_A.OUT.ALL_OUT - (float)MOTOR->DATA.SPEED_NOW;
//    MOTOR->PID_S.OUT.P_OUT = MOTOR->PID_S.IN.KP * MOTOR->PID_S.OUT.ERROR[YU_D_NOW];
//    MOTOR->PID_S.OUT.I_OUT += (MOTOR->PID_S.IN.KI * MOTOR->PID_S.OUT.ERROR[YU_D_NOW]);
//    MOTOR->PID_S.OUT.D_OUT = MOTOR->PID_S.IN.KD * (MOTOR->PID_S.OUT.ERROR[YU_D_NOW] - MOTOR->PID_S.OUT.ERROR[YU_D_LAST]);
//    MOTOR->PID_S.OUT.I_OUT = YU_D_MATH_LIMIT(MOTOR->PID_S.IN.I_LIT, -MOTOR->PID_S.IN.I_LIT, MOTOR->PID_S.OUT.I_OUT);
//    MOTOR->PID_S.OUT.ALL_OUT = MOTOR->PID_S.OUT.P_OUT + MOTOR->PID_S.OUT.I_OUT + MOTOR->PID_S.OUT.D_OUT;
//    MOTOR->PID_S.OUT.ALL_OUT = YU_D_MATH_LIMIT(MOTOR->PID_S.IN.ALL_LIT, -MOTOR->PID_S.IN.ALL_LIT, MOTOR->PID_S.OUT.ALL_OUT);
//
//    // 数据迭代
//    MOTOR->PID_S.OUT.ERROR[YU_D_LAST] = MOTOR->PID_S.OUT.ERROR[YU_D_NOW];
//}


/**
 * PID初始化，传配置文件
 * @param MOTOR
 * @param MOD
 * @return
 */
//int YU_F_MOTOR_PID_INIT(YU_TYPEDEF_MOTOR *MOTOR, int MOD)
//{
//    // 配置文件路径
//    std::string FILENAME = YU_D_CONFIG_PATH;
//    // 解析文件
//    YU_TYPEDEF_PID_CONFIG CONFIG = YU_F_PARSE_CONFIG(FILENAME);
//
//    switch (MOD)
//    {
//        case 1:
//        {
//            // 云台 YAW PID 数据
//            MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_A.IN.KP = std::stof(CONFIG["YAW"]["PID_A_P"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_A.IN.KI = std::stof(CONFIG["YAW"]["PID_A_I"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_A.IN.KD = std::stof(CONFIG["YAW"]["PID_A_D"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_A.IN.I_LIT = std::stof(CONFIG["YAW"]["PID_A_I_L"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_A.IN.ALL_LIT = std::stof(CONFIG["YAW"]["PID_A_L"]);
//
//            MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_S.IN.KP = std::stof(CONFIG["YAW"]["PID_S_P"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_S.IN.KI = std::stof(CONFIG["YAW"]["PID_S_I"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_S.IN.KD = std::stof(CONFIG["YAW"]["PID_S_D"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_S.IN.I_LIT = std::stof(CONFIG["YAW"]["PID_S_I_L"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_YAW].PID_S.IN.ALL_LIT = std::stof(CONFIG["YAW"]["PID_S_A_L"]);
//
//            printf("云台 YAW OK\n");
//
//            // 云台 PITCH PID 数据
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_A.IN.KP = std::stof(CONFIG["PIT"]["PID_A_P"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_A.IN.KI = std::stof(CONFIG["PIT"]["PID_A_I"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_A.IN.KD = std::stof(CONFIG["PIT"]["PID_A_D"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_A.IN.I_LIT = std::stof(CONFIG["PIT"]["PID_A_I_L"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_A.IN.ALL_LIT = std::stof(CONFIG["PIT"]["PID_A_A_L"]);
//
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_S.IN.KP = std::stof(CONFIG["PIT"]["PID_S_P"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_S.IN.KI = std::stof(CONFIG["PIT"]["PID_S_I"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_S.IN.KD = std::stof(CONFIG["PIT"]["PID_S_D"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_S.IN.I_LIT = std::stof(CONFIG["PIT"]["PID_S_I_L"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].PID_S.IN.ALL_LIT = std::stof(CONFIG["PIT"]["PID_S_A_L"]);
//
//            printf("云台 PITCH OK\n");
//
//            printf("YAW初始角度 INIT\n");
//            MOTOR[YU_D_MOTOR_GIMBAL_YAW].DATA.ANGLE_INIT = std::stoi(CONFIG["GIMBAL"]["YAW_INIT_ANGLE"]);
//            printf("YAW初始角度初始化 OK\n");
//
//            printf("PIT限幅初始化\n");
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].DATA.ANGLE_MAX = std::stoi(CONFIG["GIMBAL"]["PIT_MAX_ANGLE"]);
//            MOTOR[YU_D_MOTOR_GIMBAL_PIT].DATA.ANGLE_MIN = std::stoi(CONFIG["GIMBAL"]["PIT_MIN_ANGLE"]);
//            printf("PIT限幅初始化 OK\n");
//
//            printf("发射PID参数初始化\n");
//            //  发射机构 拨弹电机PID数据
//            MOTOR[YU_D_MOTOR_ATTACK_G].PID_S.IN.KP = std::stof(CONFIG["ATTACK_G"]["PID_S_P"]);
//            MOTOR[YU_D_MOTOR_ATTACK_G].PID_S.IN.KI = std::stof(CONFIG["ATTACK_G"]["PID_S_I"]);
//            MOTOR[YU_D_MOTOR_ATTACK_G].PID_S.IN.I_LIT = std::stof(CONFIG["ATTACK_G"]["PID_S_I_L"]);
//            MOTOR[YU_D_MOTOR_ATTACK_G].PID_S.IN.KD = std::stof(CONFIG["ATTACK_G"]["PID_S_D"]);
//            MOTOR[YU_D_MOTOR_ATTACK_G].PID_S.IN.ALL_LIT = std::stof(CONFIG["ATTACK_G"]["PID_S_A_L"]);
//
//            MOTOR[YU_D_MOTOR_ATTACK_G].PID_A.IN.KP = std::stof(CONFIG["ATTACK_G"]["PID_P_P"]);
//            MOTOR[YU_D_MOTOR_ATTACK_G].PID_A.IN.KI = std::stof(CONFIG["ATTACK_G"]["PID_P_I"]);
//            MOTOR[YU_D_MOTOR_ATTACK_G].PID_A.IN.I_LIT = std::stof(CONFIG["ATTACK_G"]["PID_P_I_L"]);
//            MOTOR[YU_D_MOTOR_ATTACK_G].PID_A.IN.KD = std::stof(CONFIG["ATTACK_G"]["PID_P_D"]);
//            MOTOR[YU_D_MOTOR_ATTACK_G].PID_A.IN.ALL_LIT = std::stof(CONFIG["ATTACK_G"]["PID_P_A_L"]);
//            printf("拨弹电机 OK\n");
//
//            //  发射机构 摩擦轮PID数据
//            MOTOR[YU_D_MOTOR_ATTACK_L].PID_S.IN.KP = std::stof(CONFIG["ATTACK_LR"]["PID_S_P"]);
//            MOTOR[YU_D_MOTOR_ATTACK_L].PID_S.IN.KI = std::stof(CONFIG["ATTACK_LR"]["PID_S_I"]);
//            MOTOR[YU_D_MOTOR_ATTACK_L].PID_S.IN.I_LIT = std::stof(CONFIG["ATTACK_LR"]["PID_S_I_L"]);
//            MOTOR[YU_D_MOTOR_ATTACK_L].PID_S.IN.KD = std::stof(CONFIG["ATTACK_LR"]["PID_S_D"]);
//            MOTOR[YU_D_MOTOR_ATTACK_L].PID_S.IN.ALL_LIT = std::stof(CONFIG["ATTACK_LR"]["PID_S_A_L"]);
//
//            MOTOR[YU_D_MOTOR_ATTACK_L].PID_A.IN.KP = std::stof(CONFIG["ATTACK_LR"]["PID_P_P"]);
//            MOTOR[YU_D_MOTOR_ATTACK_L].PID_A.IN.KI = std::stof(CONFIG["ATTACK_LR"]["PID_P_I"]);
//            MOTOR[YU_D_MOTOR_ATTACK_L].PID_A.IN.I_LIT = std::stof(CONFIG["ATTACK_LR"]["PID_P_I_L"]);
//            MOTOR[YU_D_MOTOR_ATTACK_L].PID_A.IN.KD = std::stof(CONFIG["ATTACK_LR"]["PID_P_D"]);
//            MOTOR[YU_D_MOTOR_ATTACK_L].PID_A.IN.ALL_LIT = std::stof(CONFIG["ATTACK_LR"]["PID_P_A_L"]);
//            printf("摩擦轮 左 OK\n");
//
//            MOTOR[YU_D_MOTOR_ATTACK_R].PID_S.IN.KP = std::stof(CONFIG["ATTACK_LR"]["PID_S_P"]);
//            MOTOR[YU_D_MOTOR_ATTACK_R].PID_S.IN.KI = std::stof(CONFIG["ATTACK_LR"]["PID_S_I"]);
//            MOTOR[YU_D_MOTOR_ATTACK_R].PID_S.IN.I_LIT = std::stof(CONFIG["ATTACK_LR"]["PID_S_I_L"]);
//            MOTOR[YU_D_MOTOR_ATTACK_R].PID_S.IN.KD = std::stof(CONFIG["ATTACK_LR"]["PID_S_D"]);
//
//            MOTOR[YU_D_MOTOR_ATTACK_R].PID_A.IN.KP = std::stof(CONFIG["ATTACK_LR"]["PID_P_P"]);
//            MOTOR[YU_D_MOTOR_ATTACK_R].PID_A.IN.KI = std::stof(CONFIG["ATTACK_LR"]["PID_P_I"]);
//            MOTOR[YU_D_MOTOR_ATTACK_R].PID_A.IN.KD = std::stof(CONFIG["ATTACK_LR"]["PID_P_D"]);
//            MOTOR[YU_D_MOTOR_ATTACK_R].PID_A.IN.ALL_LIT = std::stof(CONFIG["ATTACK_LR"]["PID_P_A_L"]);
//            printf("摩擦轮 右 OK\n");
//
//            break;
//
//        }
//        case 2:
//        {
//            printf("底盘PID参数初始化\n");
//            // 底盘PID数据
//            MOTOR[YU_D_MOTOR_CHASSIS_1].PID_S.IN.KP = std::stof(CONFIG["CHASSIS"]["PID_S_P"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_1].PID_S.IN.KI = std::stof(CONFIG["CHASSIS"]["PID_S_I"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_1].PID_S.IN.KD = std::stof(CONFIG["CHASSIS"]["PID_S_D"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_1].PID_S.IN.ALL_LIT = std::stof(CONFIG["CHASSIS"]["PID_S_A_L"]);
//
//            MOTOR[YU_D_MOTOR_CHASSIS_1].PID_C.IN.KP = std::stof(CONFIG["CHASSIS"]["PID_C_P"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_1].PID_C.IN.KI = std::stof(CONFIG["CHASSIS"]["PID_C_I"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_1].PID_C.IN.KD = std::stof(CONFIG["CHASSIS"]["PID_C_D"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_1].PID_C.IN.ALL_LIT = std::stof(CONFIG["CHASSIS"]["PID_C_A_L"]);
//            printf("底盘01 OK\n");
//
//            MOTOR[YU_D_MOTOR_CHASSIS_2].PID_S.IN.KP = std::stof(CONFIG["CHASSIS"]["PID_S_P"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_2].PID_S.IN.KI = std::stof(CONFIG["CHASSIS"]["PID_S_I"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_2].PID_S.IN.KD = std::stof(CONFIG["CHASSIS"]["PID_S_D"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_2].PID_S.IN.ALL_LIT = std::stof(CONFIG["CHASSIS"]["PID_S_A_L"]);
//
//            MOTOR[YU_D_MOTOR_CHASSIS_2].PID_C.IN.KP = std::stof(CONFIG["CHASSIS"]["PID_C_P"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_2].PID_C.IN.KI = std::stof(CONFIG["CHASSIS"]["PID_C_I"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_2].PID_C.IN.KD = std::stof(CONFIG["CHASSIS"]["PID_C_D"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_2].PID_C.IN.ALL_LIT = std::stof(CONFIG["CHASSIS"]["PID_C_A_L"]);
//            printf("底盘02 OK");
//
//            MOTOR[YU_D_MOTOR_CHASSIS_3].PID_S.IN.KP = std::stof(CONFIG["CHASSIS"]["PID_S_P"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_3].PID_S.IN.KI = std::stof(CONFIG["CHASSIS"]["PID_S_I"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_3].PID_S.IN.KD = std::stof(CONFIG["CHASSIS"]["PID_S_D"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_3].PID_S.IN.ALL_LIT = std::stof(CONFIG["CHASSIS"]["PID_S_A_L"]);
//
//            MOTOR[YU_D_MOTOR_CHASSIS_3].PID_C.IN.KP = std::stof(CONFIG["CHASSIS"]["PID_C_P"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_3].PID_C.IN.KI = std::stof(CONFIG["CHASSIS"]["PID_C_I"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_3].PID_C.IN.KD = std::stof(CONFIG["CHASSIS"]["PID_C_D"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_3].PID_C.IN.ALL_LIT = std::stof(CONFIG["CHASSIS"]["PID_C_A_L"]);
//            printf("底盘03 OK");
//
//            MOTOR[YU_D_MOTOR_CHASSIS_4].PID_S.IN.KP = std::stof(CONFIG["CHASSIS"]["PID_S_P"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_4].PID_S.IN.KI = std::stof(CONFIG["CHASSIS"]["PID_S_I"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_4].PID_S.IN.KD = std::stof(CONFIG["CHASSIS"]["PID_S_D"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_4].PID_S.IN.ALL_LIT = std::stof(CONFIG["CHASSIS"]["PID_S_A_L"]);
//
//            MOTOR[YU_D_MOTOR_CHASSIS_4].PID_C.IN.KP = std::stof(CONFIG["CHASSIS"]["PID_C_P"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_4].PID_C.IN.KI = std::stof(CONFIG["CHASSIS"]["PID_C_I"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_4].PID_C.IN.KD = std::stof(CONFIG["CHASSIS"]["PID_C_D"]);
//            MOTOR[YU_D_MOTOR_CHASSIS_4].PID_C.IN.ALL_LIT = std::stof(CONFIG["CHASSIS"]["PID_C_A_L"]);
//            printf("底盘04 OK");
//#ifdef YU_BUILD_DEBUG
//            for (int I = 0; I < 4; ++I)
//            {
//                memcpy(&YU_V_DEBUG[I].PID_S.IN, &MOTOR[I].PID_S.IN, sizeof(MOTOR[I].PID_S.IN));
//                memcpy(&YU_V_DEBUG[I].PID_C.IN, &MOTOR[I].PID_C.IN, sizeof(MOTOR[I].PID_C.IN));
//            }
//            YU_D_LOG_NOTICE("成功-初始化DEBUG变量的底盘PID的值");
//#endif
//            break;
//
//        }
//        default:break;
//        }
//    return true;
//}

/**
 * @brief               自己写的PID初始化，不用读配置文件
 * @details             传入PID基本参数， 通过数组
 * @param PID       :   PID 指针，  形式 : MOTOR->PID
 * @param PID_V     :   5个float, 参数和限幅
 * @date                2024-03-12
 * @author              YU
 */
void YU_T_PID_INIT(YU_TYPEDEF_MOTOR_PID *PID, const float *PID_V)
{
    if (PID == nullptr || PID_V == nullptr)
    {
        return;
    }

    PID->IN.KP      = PID_V[0];
    PID->IN.KI      = PID_V[1];
    PID->IN.KD      = PID_V[2];
    PID->IN.I_LIT   = PID_V[3];
    PID->IN.ALL_LIT = PID_V[4];

    PID->OUT.ERROR[0] = 0.0f;
    PID->OUT.ERROR[1] = 0.0f;
}

/**
 * @brief               PID 解算
 * @details             PID 解算
 * @param PID       :   PID 指针，  形式 : MOTOR->PID
 * @param TARGET    :   目标值
 * @param REALVAL   :   实际值
 * @return
 */
float YU_T_PID_CAL(YU_TYPEDEF_MOTOR_PID *PID, float TARGET, float REALVAL)
{
    if (PID == nullptr)
    {
        return 0.0f;
    }

    PID->OUT.ERROR[YU_D_NOW] = TARGET - REALVAL;

    PID->OUT.P_OUT   = PID->IN.KP * PID->OUT.ERROR[YU_D_NOW];
    PID->OUT.I_OUT  += PID->IN.KI * PID->OUT.ERROR[YU_D_NOW];
//    PID->OUT.I_OUT   = YU_D_MATH_LIMIT(PID->IN.I_LIT,   -PID->IN.I_LIT,   PID->OUT.I_OUT);

    PID->OUT.D_OUT   = PID->IN.KD * (PID->OUT.ERROR[YU_D_NOW] - PID->OUT.ERROR[YU_D_LAST]);
    PID->OUT.ALL_OUT = PID->OUT.P_OUT + PID->OUT.I_OUT + PID->OUT.D_OUT;

    PID->OUT.ALL_OUT = YU_D_MATH_LIMIT(PID->IN.ALL_LIT, -PID->IN.ALL_LIT, PID->OUT.ALL_OUT);

    PID->OUT.ERROR[YU_D_LAST] = PID->OUT.ERROR[YU_D_NOW];

    return PID->OUT.ALL_OUT;
}
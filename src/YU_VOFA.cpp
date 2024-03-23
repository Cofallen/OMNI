#include "YU_THREAD.h"
#include "YU_MOTOR.h"
#include "YU_PID.h"
#include "YU_VOFA.h"

#include <unordered_map>


/**
 * @details 将VOFA接收的数据分成 NAME PARAM
 * @param RECV
 */
void YU_F_VOFA_PARSE(YU_TYPEDEF_RECV_UNION *RECV)
{
    std::string ITER = RECV->ALL;
    auto POS = ITER.find(':');  // 0x3A
    auto NAME_RAW= ITER.substr(0, POS);
    auto VALUE_RAW = ITER.substr(POS + 1);
    auto PARAM = std::stof(VALUE_RAW);

    strcpy(RECV->DATA.NAME,NAME_RAW.c_str());
    RECV->DATA.PARAM = PARAM;

//    printf("NAME = %s  PARAM = %f\n",RECV->DATA.NAME, RECV->DATA.PARAM);

}


/**
 * @details 简单赋值，将MOTOR数据赋给单个 DEBUG
 * @details 其他方法TODO : map映射 | 共用地址
 * @param DEBUG
 * @param MOTOR
 * @author YU
 * @date 2024-03-21
 */
void YU_F_VOFA_DEBUG_CAL(YU_TYPEDEF_DEBUG *DEBUG, YU_TYPEDEF_MOTOR *MOTOR)
{
    DEBUG->MOTOR_DATA.AIM = MOTOR->DATA.AIM;
    DEBUG->MOTOR_DATA.ANGLE = MOTOR->DATA.ANGLE_NOW;
    DEBUG->MOTOR_DATA.LAPS = MOTOR->DATA.LAPS;
    DEBUG->MOTOR_DATA.CURRENT = MOTOR->DATA.CURRENT;
    DEBUG->MOTOR_DATA.SPEED = MOTOR->DATA.SPEED_NOW;

    DEBUG->PID_S.OUT.ERROR[YU_D_NOW] = MOTOR->PID_S.OUT.ERROR[YU_D_NOW];
    DEBUG->PID_S.OUT.ERROR[YU_D_LAST] = MOTOR->PID_S.OUT.ERROR[YU_D_LAST];
    DEBUG->PID_S.OUT.P_OUT = MOTOR->PID_S.OUT.P_OUT;
    DEBUG->PID_S.OUT.I_OUT = MOTOR->PID_S.OUT.I_OUT;
    DEBUG->PID_S.OUT.D_OUT = MOTOR->PID_S.OUT.D_OUT;
    DEBUG->PID_S.OUT.ALL_OUT = MOTOR->PID_S.OUT.ALL_OUT;
    DEBUG->PID_S.IN.KP = MOTOR->PID_S.IN.KP;
    DEBUG->PID_S.IN.KI = MOTOR->PID_S.IN.KI;
    DEBUG->PID_S.IN.KD = MOTOR->PID_S.IN.KD;
    DEBUG->PID_S.IN.I_LIT = MOTOR->PID_S.IN.I_LIT;
    DEBUG->PID_S.IN.ALL_LIT = MOTOR->PID_S.IN.ALL_LIT;

    DEBUG->PID_A.OUT.ERROR[YU_D_NOW] = MOTOR->PID_A.OUT.ERROR[YU_D_NOW];
    DEBUG->PID_A.OUT.ERROR[YU_D_LAST] = MOTOR->PID_A.OUT.ERROR[YU_D_LAST];
    DEBUG->PID_A.OUT.P_OUT = MOTOR->PID_A.OUT.P_OUT;
    DEBUG->PID_A.OUT.I_OUT = MOTOR->PID_A.OUT.I_OUT;
    DEBUG->PID_A.OUT.D_OUT = MOTOR->PID_A.OUT.D_OUT;
    DEBUG->PID_A.OUT.ALL_OUT = MOTOR->PID_A.OUT.ALL_OUT;
    DEBUG->PID_A.IN.KP = MOTOR->PID_A.IN.KP;
    DEBUG->PID_A.IN.KI = MOTOR->PID_A.IN.KI;
    DEBUG->PID_A.IN.KD = MOTOR->PID_A.IN.KD;
    DEBUG->PID_A.IN.I_LIT = MOTOR->PID_A.IN.I_LIT;
    DEBUG->PID_A.IN.ALL_LIT = MOTOR->PID_A.IN.ALL_LIT;

    DEBUG->PID_C.OUT.ERROR[YU_D_NOW] = MOTOR->PID_C.OUT.ERROR[YU_D_NOW];
    DEBUG->PID_C.OUT.ERROR[YU_D_LAST] = MOTOR->PID_C.OUT.ERROR[YU_D_LAST];
    DEBUG->PID_C.OUT.P_OUT = MOTOR->PID_C.OUT.P_OUT;
    DEBUG->PID_C.OUT.I_OUT = MOTOR->PID_C.OUT.I_OUT;
    DEBUG->PID_C.OUT.D_OUT = MOTOR->PID_C.OUT.D_OUT;
    DEBUG->PID_C.OUT.ALL_OUT = MOTOR->PID_C.OUT.ALL_OUT;
    DEBUG->PID_C.IN.KP = MOTOR->PID_C.IN.KP;
    DEBUG->PID_C.IN.KI = MOTOR->PID_C.IN.KI;
    DEBUG->PID_C.IN.KD = MOTOR->PID_C.IN.KD;
    DEBUG->PID_C.IN.I_LIT = MOTOR->PID_C.IN.I_LIT;
    DEBUG->PID_C.IN.ALL_LIT = MOTOR->PID_C.IN.ALL_LIT;

}

/**
 * @details 调用赋值函数，将电机要发送数据存到DEBUG[]
 */
void YU_F_VOFA_DEBUG()
{
    if (MOTOR_TYPE == 9) return;

    switch (MOTOR_TYPE) 
    {
        case YU_D_VOFA_C1: YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_C1], &YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_1]);
            break;
        case YU_D_VOFA_C2: YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_C2], &YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_2]);
            break;
        case YU_D_VOFA_C3: YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_C3], &YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_3]);
            break;
        case YU_D_VOFA_C4: YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_C4], &YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_4]);
            break;
        case YU_D_VOFA_GY: YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_GY], &YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_YAW]);
            break;
        case YU_D_VOFA_GP: YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_GP], &YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_PIT]);
            break;
        default:
            break;
    }
}


static void A_P(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_A.IN.KP = PARAM;}
static void A_I(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_A.IN.KI = PARAM;}
static void A_D(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_A.IN.KD = PARAM;}
static void A_ILIT(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_A.IN.I_LIT = PARAM;}
static void A_ALIT(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_A.IN.ALL_LIT = PARAM;}

static void S_P(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_S.IN.KP = PARAM;}
static void S_I(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_S.IN.KI = PARAM;}
static void S_D(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_S.IN.KD = PARAM;}
static void S_ILIT(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_S.IN.I_LIT = PARAM;}
static void S_ALIT(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_S.IN.ALL_LIT = PARAM;}

static void C_P(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_C.IN.KP = PARAM;}
static void C_I(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_C.IN.KI = PARAM;}
static void C_D(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_C.IN.KD = PARAM;}
static void C_ILIT(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_C.IN.I_LIT = PARAM;}
static void C_ALIT(YU_TYPEDEF_MOTOR *MOTOR, float PARAM) {MOTOR->PID_C.IN.ALL_LIT = PARAM;}


/**
 * @details 获取MOTOR_TYPE, 将PARAM传到电机数据中
 * @param RECV
 */
void YU_F_VOFA_ASSIGN(YU_TYPEDEF_RECV_UNION *RECV)
{
    std::string OUTLINE = RECV->DATA.NAME;
    auto POS_1 = OUTLINE.find('_');             // HEAD_TAIL
    auto HEAD = OUTLINE.substr(0, POS_1);  //   C1_S_P
    auto TAIL = OUTLINE.substr(POS_1+1);

    // C1 C2 C3 C4 GY GP  // A S C  // P I D ILIT ALIT
    static const std::unordered_map<std::string, int> MOTOR_MAP{
            {"C1", YU_D_VOFA_C1},
            {"C2", YU_D_VOFA_C2},
            {"C3", YU_D_VOFA_C3},
            {"C4", YU_D_VOFA_C4},
            {"GY", YU_D_VOFA_GY},
            {"GP", YU_D_VOFA_GP}
    };
    auto IT_HEAD = MOTOR_MAP.find(HEAD);
    if (IT_HEAD != MOTOR_MAP.end())
    {
        MOTOR_TYPE = IT_HEAD->second;
    } else
    {
        MOTOR_TYPE = 9;   // 用其他不用的数据DEBUG[9]{}
    }

//    printf("MOTOR_TYPE:  %d\n",MOTOR_TYPE);

    // 赋值
    static const std::unordered_map<std::string, void(*)(YU_TYPEDEF_MOTOR *, float)> WRITE_MAP {
            {"A_P", A_P}, {"A_I", A_I}, {"A_D", A_D}, {"A_ILIT", A_ILIT}, {"A_ALIT", A_ALIT},
            {"S_P", S_P}, {"S_I", S_I}, {"S_D", S_D}, {"S_ILIT", S_ILIT}, {"S_ALIT", S_ALIT},
            {"C_P", C_P}, {"C_I", C_I}, {"C_D", C_D}, {"C_ILIT", C_ILIT}, {"C_ALIT", C_ALIT}
    };

    auto IT_TAIL = WRITE_MAP.find(TAIL);
    if (IT_TAIL != WRITE_MAP.end())
    {
        switch (MOTOR_TYPE)
        {
            case YU_D_VOFA_C1: IT_TAIL->second(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_1], RECV->DATA.PARAM);
                break;
            case YU_D_VOFA_C2: IT_TAIL->second(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_2], RECV->DATA.PARAM);
                break;
            case YU_D_VOFA_C3: IT_TAIL->second(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_3], RECV->DATA.PARAM);
                break;
            case YU_D_VOFA_C4: IT_TAIL->second(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_4], RECV->DATA.PARAM);
                break;
            case YU_D_VOFA_GY: IT_TAIL->second(&YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_YAW], RECV->DATA.PARAM);
                break;
            case YU_D_VOFA_GP: IT_TAIL->second(&YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_PIT], RECV->DATA.PARAM);
                break;
            case 9:    // 空
                break;
            default:
                break;
        }
    }
    else
    {
        // TAIL 数据有误
    }

//    printf("KP: %f\n", YU_V_MOTOR_CHASSIS[0].PID_S.IN.KP);

}






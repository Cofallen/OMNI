#include "YU_MOTOR.h"
#include "YU_DEFINE.h"

/**
 * @brief 电机CAN解算函数
 * @param MOTOR 电机结构体指针
 * @param CAN_DATA CAN 接收数据组指针
 * @param TYPE 电机ID
 * @param STATUS_ID
 */
void YU_F_MOTOR_CAN_CAL(YU_TYPEDEF_MOTOR *MOTOR,const uint8_t *CAN_DATA,uint8_t TYPE,uint8_t STATUS_ID)
{
    // NOW->LAST 转换
    MOTOR->DATA.ANGLE_LAST = MOTOR->DATA.ANGLE_NOW;
    MOTOR->DATA.SPEED_LAST = MOTOR->DATA.SPEED_NOW;

    // 解算
    MOTOR->DATA.ANGLE_NOW = (int16_t)(((CAN_DATA[0] << 8) | CAN_DATA[1]) & 0xFFFF);
    MOTOR->DATA.SPEED_NOW = (int16_t)(((CAN_DATA[2] << 8) | CAN_DATA[3]) & 0xFFFF);
    MOTOR->DATA.CURRENT   = (int16_t)(((CAN_DATA[4] << 8) | CAN_DATA[5]) & 0xFFFF);
    MOTOR->DATA.TEMP      = (int16_t)(((CAN_DATA[6] << 8) | CAN_DATA[7]) & 0xFFFF);
    
    // 离线监测先不写


    // 电机判断和解算
    switch (TYPE) {
        case YU_D_MOTOR_TYPE_3508:
        {
        }
            break;

        case YU_D_MOTOR_TYPE_3510:
        {
        }
            break;

        case YU_D_MOTOR_TYPE_2006:
        {
        }
            break;

        // 瑞的没看明白，先自己写
        case YU_D_MOTOR_TYPE_6020:
        {

//            auto ANGLE_ERROR_INIT = (int16_t) (MOTOR->DATA.ANGLE_NOW - MOTOR->DATA.ANGLE_INIT);
//            auto ANGLE_ERROR_NOW_LAST = (int16_t) (MOTOR->DATA.ANGLE_NOW - MOTOR->DATA.ANGLE_LAST);
//
//            if (ANGLE_ERROR_NOW_LAST < -4096)
//            {
//                MOTOR->DATA.LAPS++;
//            } else if (ANGLE_ERROR_NOW_LAST > 4096)
//            {
//                MOTOR->DATA.LAPS--;
//            }
//            if ((MOTOR->DATA.LAPS > 32) | (MOTOR->DATA.LAPS < -32))
//            {
//                MOTOR->DATA.LAPS = 0;
//                MOTOR->DATA.AIM = MOTOR->DATA.ANGLE_NOW;
//            }
//            if (ANGLE_ERROR_INIT < -4000)
//            {
//                ANGLE_ERROR_INIT -= 8192;
//            } else if (ANGLE_ERROR_INIT > 4096)
//            {
//                ANGLE_ERROR_INIT += 8192;
//            }
//            MOTOR->DATA.ANGLE_RELATIVE = ANGLE_ERROR_INIT;
//            MOTOR->DATA.ANGLE_INFINITE = (int32_t) ((MOTOR->DATA.LAPS << 13) + MOTOR->DATA.ANGLE_NOW);
//        }

            if (MOTOR->DATA.ANGLE_NOW - MOTOR->DATA.ANGLE_LAST < -4096)
            {
                MOTOR->DATA.LAPS++;
            }
            else if (MOTOR->DATA.ANGLE_NOW - MOTOR->DATA.ANGLE_LAST > 4096)
            {
                MOTOR->DATA.LAPS--;
            }

            MOTOR->DATA.ANGLE_INFINITE = MOTOR->DATA.LAPS * 8192 + MOTOR->DATA.ANGLE_NOW;

        }
            break;

        default:
            break;
    }

}

/**
 * @brief 电机清空函数
 * @param MOTOR 电机结构体指针
 * @param TYPE 电机ID
 */
void YU_F_MOTOR_CLEAR(YU_TYPEDEF_MOTOR *MOTOR, uint8_t *TYPE)
{
    MOTOR->DATA.LAPS = 0;
    MOTOR->PID_A.OUT.I_OUT = MOTOR->PID_A.OUT.ALL_OUT = 0;
    MOTOR->PID_S.OUT.I_OUT = MOTOR->PID_S.OUT.ALL_OUT = 0;
    MOTOR->PID_C.OUT.I_OUT = MOTOR->PID_C.OUT.ALL_OUT = 0;

}

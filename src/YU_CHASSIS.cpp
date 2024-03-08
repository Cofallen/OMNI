//
// Created by 17273 on 2024/3/8.
//

#include "YU_CHASSIS.h"
#include "YU_PID.h"
#include <cmath>

/**
 * @brief 地盘功率分配
 * @param TURNFAACTOR
 * @return
 */
float *YU_F_CALCULATE_WHEELPOWERS(const float TURNFAACTOR)
{
    static float MAX_TURNFACTOR = 3640;

    static float COMPONENT[2] = {0};
    // 根据转向因子调整X和Y方向的功率分配
    COMPONENT[0] = ((MAX_TURNFACTOR - TURNFAACTOR) / MAX_TURNFACTOR);
    COMPONENT[1] = (1 - (MAX_TURNFACTOR - TURNFAACTOR) / MAX_TURNFACTOR);

    return COMPONENT;
}


void YU_F_CHASSIS(YU_TYPEDEF_MOTOR *MOTOR)
{}
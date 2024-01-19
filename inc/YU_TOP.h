//
// Created by 17273 on 2024/1/19.
//

#ifndef DEMO_YU_TOP_H
#define DEMO_YU_TOP_H

#include <cstdio>
#include <cstdint>
#include <cstring>

typedef struct RUI_TYPEDEF_TOP
{
    float REALITY_ANGLE;            //真实角度//带圈数
    int16_t YAW_ANGLE[2];
    float YAW_ANGLE_F;
    int16_t YAW_SPEED[2];
    int16_t PIT_ANGLE[2];
    float PIT_ANGLE_F;
    int16_t PIT_SPEED[2];
    int16_t ROUND;

} YU_TYPEDEF_TOP;

#endif //DEMO_YU_TOP_H

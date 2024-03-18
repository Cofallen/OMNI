//
// Created by 17273 on 2024/2/27.
//

#ifndef DEMO_YU_PID_H
#define DEMO_YU_PID_H

#include "YU_MOTOR.h"

//void YU_F_MOTOR_PID_AS(YU_TYPEDEF_MOTOR *MOTOR);
//int YU_F_MOTOR_PID_INIT(YU_TYPEDEF_MOTOR *MOTOR, int MOD);

void YU_T_PID_INIT(YU_TYPEDEF_MOTOR_PID *PID, const float *PID_V);
float YU_T_PID_CAL(YU_TYPEDEF_MOTOR_PID *PID, float TARGET, float REALVAL);


#endif //DEMO_YU_PID_H

//
// Created by 17273 on 2024/4/1.
//

#ifndef DEMO_YU_ATTACK_H
#define DEMO_YU_ATTACK_H

#include "YU_MOTOR.h"
#include "YU_UART.h"

extern YU_TYPEDEF_MOTOR YU_V_MOTOR_ATTACK[3];


bool YU_F_ATTACK_INIT();
float YU_F_ATTACK_JAM_AIM(YU_TYPEDEF_MOTOR *MOTOR, YU_TYPEDEF_DBUS *DBUS);
bool YU_F_ATTACK_JAM_CHECK(YU_TYPEDEF_MOTOR *MOTOR);
[[noreturn]] void YU_F_THREAD_ATTACK();


#endif //DEMO_YU_ATTACK_H

//
// Created by 17273 on 2024/3/14.
//

#ifndef DEMO_YU_GIMBAL_H
#define DEMO_YU_GIMBAL_H

#include "YU_UART.h"
#include "YU_MOTOR.h"

extern float GIMBAL_OUT[2];
extern float GIMBAL_TARGET[2];

[[noreturn]]void YU_F_GIMBAL(YU_TYPEDEF_DBUS *DBUS);
[[noreturn]]void YU_F_GIMBAL_SEND(YU_TYPEDEF_MOTOR *MOTOR, uint32_t ID)

#endif //DEMO_YU_GIMBAL_H

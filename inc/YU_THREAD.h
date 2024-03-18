//
// Created by 17273 on 2024/3/12.
//

#ifndef DEMO_YU_THREAD_H
#define DEMO_YU_THREAD_H

#include "YU_UART.h"

extern YU_TYPEDEF_DBUS YU_V_DBUS;
extern YU_TYPEDEF_MONITOR_DBUS YU_V_MONITOR_DBUS;


[[noreturn]] void YU_F_THREAD_CHASSIS_MECANUM();
[[noreturn]] void YU_F_THREAD_TEST();
[[noreturn]] void YU_F_THREAD_MONITOR();
[[noreturn]] void YU_F_THREAD_GIMBAL();


#endif //DEMO_YU_THREAD_H

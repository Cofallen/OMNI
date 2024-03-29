//
// Created by 17273 on 2024/3/8.
//

#ifndef DEMO_YU_ROOT_INIT_H
#define DEMO_YU_ROOT_INIT_H

#include <csignal>
#include <cstdint>
#include "YU_DEFINE.h"

//struct YU_TYPEDEF_MONITOR
//{
//    int8_t STATUS[2][YU_D_STATUS_NUM];
//    uint16_t TIME[YU_D_STATUS_NUM];
//};

//struct YU_TYPEDEF_MONITOR YU_V_MONITOR{ };

// 自己写的遥控监视
typedef struct
{
    int64_t TIME;
    int8_t STATUS;
}YU_TYPEDEF_MONITOR_DBUS;


#endif //DEMO_YU_ROOT_INIT_H

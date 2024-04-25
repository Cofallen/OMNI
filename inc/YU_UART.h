//
// Created by 17273 on 2024/3/8.
//

#ifndef YU_UART_H
#define YU_UART_H

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <asm-generic/termbits.h>
#include <linux/serial.h>
#include <sys/ioctl.h>

#include "YU_DEFINE.h"
#include "YU_ROOT_INIT.h"

typedef struct YU_TYPEDEF_DBUS
{

    uint8_t R_FLAG;
    uint8_t L_FLAG;

    //遥控
    struct
    {
        int16_t CH0_int16;
        int16_t CH1_int16;
        int16_t CH2_int16;
        int16_t CH3_int16;
        int16_t DIR_int16;
        uint8_t S1_u8;
        uint8_t S2_u8;

        int8_t Error_int8;
    } REMOTE;
    //鼠标
    struct
    {
        float X_FLT;
        float X_FILTER; // 处理后使用的鼠标值
        float X_MAX;    // 记录鼠标的最大值用来校准鼠标
        float Y_Flt;
        float Y_FILTER; // 处理后使用的鼠标值
        float Y_MAX;    // 记录鼠标的最大值用来校准鼠标
        uint8_t R_STATE: 4;
        uint8_t L_STATE: 4;
        uint8_t R_PRESS_TIME;
        uint8_t L_PRESS_TIME;
    } MOUSE;

} YU_TYPEDEF_DBUS_;

union YU_TYPEDEF_DBUS_UNION // 使用共用体整合数据
{
    struct
    {
        // 遥控数据
        uint64_t CH0: 11;
        uint64_t CH1: 11;
        uint64_t CH2: 11;
        uint64_t CH3: 11;
        uint64_t S1: 2;
        uint64_t S2: 2;

        // 遥控滑轮
        uint64_t DIR: 11;
        uint64_t : 5;
    } DATA_NEATEN;
    // 接收到的数组
    uint8_t GET_DATA[19];
};



[[noreturn]] void YU_F_DBUS_THREAD(YU_TYPEDEF_DBUS *YU_V_DBUS);


#endif //YU_UART_H

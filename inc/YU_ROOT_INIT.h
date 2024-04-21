//
// Created by 17273 on 2024/3/8.
//

#ifndef DEMO_YU_ROOT_INIT_H
#define DEMO_YU_ROOT_INIT_H

#include <csignal>
#include <cstdint>
#include "YU_DEFINE.h"

#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

// 自己写的遥控监视
typedef struct
{
    std::mutex MUTEX;
    std::condition_variable CV;
    std::atomic<bool> STATUS;
    std::chrono::milliseconds TIMEOUT;
}YU_TYPEDEF_MONITOR_DBUS;


#endif //DEMO_YU_ROOT_INIT_H

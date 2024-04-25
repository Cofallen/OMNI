#include <iostream>
#include <thread>
#include <unistd.h>
#include "Test.h"
#include "pthread.h"

#include "../inc/main.h"
#include "Test.h"
#include "YU_CAN.h"
#include "YU_DEBUG.h"
#include "YU_DEFINE.h"
#include "YU_MATH.h"
#include "YU_MOTOR.h"
#include "YU_TOP.h"
#include "YU_THREAD.h"
#include "YU_VISION.h"

using namespace std;



int main()
{
    // 注册程序退出信号
    signal(SIGINT,YU_F_THREAD_EXIT);

    YU_F_CAN_INIT();

    thread T_UART(YU_F_DBUS_THREAD, &YU_V_DBUS);
//    thread T_CHASSIS(YU_F_THREAD_CHASSIS_MECANUM);
    thread T_MONITOR(YU_F_THREAD_MONITOR);
//    thread T_VOFA(YU_F_THREAD_VOFA);
//    thread T_TEST(YU_F_THREAD_TEST);
    thread T_GIMBAL(YU_F_THREAD_GIMBAL);
    thread T_VISION(YU_F_THREAD_VISION, &YU_V_VISION);
//    T_TEST.join();
//    T_CHASSIS.join();
    T_UART.join();
    T_MONITOR.join();
//    T_VOFA.join();
    T_GIMBAL.join();
    T_VISION.join();

}

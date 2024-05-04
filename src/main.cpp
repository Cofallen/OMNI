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
#include "YU_ATTACK.h"

using namespace std;



int main()
{
#ifdef BUILD_RELEASE
    // 注册程序退出信号
    signal(SIGINT,YU_F_THREAD_EXIT);
    printf("release模式\n");
    sleep(2);
#endif
#ifdef BUILD_DEBUG
    printf("debug模式\n");
    sleep(2);
#endif

    YU_F_CAN_INIT();

    thread T_UART(YU_F_DBUS_THREAD, &YU_V_DBUS);
//    thread T_CHASSIS(YU_F_THREAD_CHASSIS_MECANUM);
    thread T_MONITOR(YU_F_THREAD_MONITOR);
    thread T_VOFA(YU_F_THREAD_VOFA);
//    thread T_GIMBAL(YU_F_THREAD_GIMBAL);
//    thread T_VISION(YU_F_THREAD_VISION, &YU_V_VISION);
    thread T_ATTACK(YU_F_THREAD_ATTACK);

//    T_TEST.join();
//    T_CHASSIS.join();
    T_UART.join();
    T_MONITOR.join();
    T_VOFA.join();
//    T_GIMBAL.join();
//    T_VISION.join();
    T_ATTACK.join();

//    cpu_set_t CPU_0;
//    CPU_ZERO(&CPU_0);
//    CPU_SET(0,&CPU_0);
//    pthread_setaffinity_np(T_UART.native_handle(),sizeof(cpu_set_t),&CPU_0);
//    pthread_setaffinity_np(T_MONITOR.native_handle(),sizeof(cpu_set_t),&CPU_0);
}

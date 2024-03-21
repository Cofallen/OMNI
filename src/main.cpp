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

using namespace std;

// 变量
YU_TYPEDEF_MOTOR GM6020;


uint8_t YU_C_DATA[8]{ };
uint32_t YU_C_TYPE;

// 测试线程的运行函数
void say_hello(void )
{
    cout << "Hello Runoob！" << endl;
    sleep(100);
}




int main()
{
    // 初始化
    YU_F_CAN_INIT();

//    thread T_UART(YU_F_DBUS_THREAD, &YU_V_DBUS);
//    thread T_CHASSIS(YU_F_THREAD_CHASSIS_MECANUM);
//    thread T_MONITOR(YU_F_THREAD_MONITOR);
    thread T_VOFA(YU_F_THREAD_VOFA);
//    thread T_TEST(YU_F_THREAD_TEST);

//    T_TEST.join();
//    T_CHASSIS.join();
//    T_UART.join();
//    T_MONITOR.join();
    T_VOFA.join();

}

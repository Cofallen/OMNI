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


using namespace std;

// 变量
YU_TYPEDEF_MOTOR GM6020;
YU_TYPEDEF_MOTOR MOTOR[10];

YU_TYPEDEF_TOP GM6020_TOP;
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

    while (1)
    {
         YU_F_CAN_RECV(MOTOR,&GM6020_TOP ,0);
         printf("ANGLE:%d\n",MOTOR[4].DATA.ANGLE_NOW);
         YU_F_CAN_SEND(0,0x1FF,1000,1000,1000,100);
usleep(1);
    }

}

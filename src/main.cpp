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
//    YU_F_CAN_INIT();

    while (1)
    {
        // 测试线程
        thread THREAD_1(say_hello);

//         VOFA 测试线程
        thread THREAD_2(YU_F_SOCKET_UDP_VOFA);

        // CAN1 线程 不知道要不要写 while(1)
        std::thread THREAD_3([](){
            if(YU_F_CAN_RECV(&GM6020,&GM6020_TOP,YU_D_CAN_1) != 1)
            {
                perror("CAN RECV error\n");
                exit(-1);
            }
            usleep(1);

            // 注释掉的编译不了，回家再说
//            if (YU_F_CAN_RESOLVE(&GM6020,&GM6020_TOP,YU_D_CAN_1,YU_D_CAN_ID_PIT,YU_C_DATA) != 1)
//            {
//                perror("CAN RESOLVE error\n");
//                exit(-1);
//            }
//            // 没有写遥控，先随便发点东西
//            if (YU_F_CAN_SEND(YU_D_CAN_1,YU_D_CAN_ID_PIT,5000,0,0,0) != 1)
//            {
//                perror("CAN SEND error\n");
//                exit(-1);
//            }
        });

        // VOFA 发送电机数据线程
        std::thread THREAD_4([](){
            printf("VOFA 发送电机数据线程\n");
            usleep(1);
//            YU_F_SOCKET_UDP_VOFA();
        });

        printf("c\n");

        THREAD_1.join();
        THREAD_2.join();
        THREAD_3.join();
        THREAD_4.join();
    }

}

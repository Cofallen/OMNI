#include <iostream>
#include <thread>
#include <unistd.h>
#include "Test.h"
#include "pthread.h"

using namespace std;

// 线程的运行函数
void say_hello(void )
{
    cout << "Hello Runoob！" << endl;
    sleep(100);
}

int main()
{
    while (1)
    {
        thread THREAD_1(say_hello);
        thread THREAD_2(YU_F_SOCKET_UDP_VOFA);
        printf("c\n");

        THREAD_1.join();
        THREAD_2.join();
    }
//    printf("成功进入main\n");
//    YU_F_SOCKET_UDP_VOFA();
//    printf("完成\n");
}

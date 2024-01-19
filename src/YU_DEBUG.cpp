#include "YU_DEBUG.h"
#include "YU_MOTOR.h"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <unordered_map>
#include <string>

// DEBUG 变量
struct YU_TYPEDEF_DEBUG YU_V_DEBUG[10]{ };

// UDP 数据解算，一堆看不懂的东西




// DEBUG 线程
void YU_F_DEBUG_THREAD()
{
    // 获取IP地址
    struct ifaddrs *IFADDRS;
    char IPSTR[INET_ADDRSTRLEN];

    // UDP地址结构
    struct sockaddr_in YU_U_SERVER_ADDR{},YU_U_CLIENT_ADDR;
    socklen_t YU_U_CLIENT_ADDR_LEN = sizeof (YU_U_CLIENT_ADDR);

    // UDP发送共用体
    union YU_TYPEDEF_SEND_UNION
    {
        struct
        {
            struct YU_TYPEDEF_DEBUG YU_V_DEBUG;
            unsigned char TAIL[4];
        }PACKAGE;
        char ALL[160];
    };

    YU_TYPEDEF_SEND_UNION YU_U_SEND{ };

    // UDP接收共用体
    // RUI写的：DATA 精度12.12345 没看懂精度，有时间分析
    union YU_TYPEDEF_RECV_UNION
    {
        struct
        {
            char NAME[8];
            float DATA;
            char TAIL;
        }PACKAGE;
        char ALL[16];
    };

    YU_TYPEDEF_RECV_UNION YU_U_RECV{ };

    // 包尾
    YU_U_SEND.PACKAGE.TAIL[2] = 0x80;
    YU_U_SEND.PACKAGE.TAIL[3] = 0x7f;

    printf("调试线程启动\n");

    // 创建UDP socket
    int YU_U_SERVER_FD = socket(AF_INET,SOCK_DGRAM,0);
    if (YU_U_SERVER_FD < 0)
    {
        perror("FD创建失败\n");
        exit(1);
    }

    // 设置为非阻塞模式
    int FLAGS = fcntl(YU_U_SERVER_FD,F_GETFL,0);
    if (fcntl(YU_U_SERVER_FD,F_SETFL,FLAGS | O_NONBLOCK) == -1)
    {
        perror("fcntl error\n");
        exit(-1);
    }

    // set and bind
    memset(&YU_U_SERVER_ADDR,0,sizeof (YU_U_SERVER_ADDR));
    YU_U_SERVER_ADDR.sin_family = AF_INET;
    YU_U_SERVER_ADDR.sin_port = htons(12345);
    YU_U_SERVER_ADDR.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(YU_U_SERVER_FD,(sockaddr *)&YU_U_SERVER_ADDR,(socklen_t)sizeof(YU_U_SERVER_ADDR))<0)
    {
        perror("bind error\n");
        close(YU_U_SERVER_FD);
        exit(1);
    }

    // ip 地址获取
    if (getifaddrs(&IFADDRS) == -1)
    {
        perror("ip地址获取失败\n");
        exit(1);
    }

    // 暂时看不懂，如何获取ip地址
//    for (; IFADDRS != nullptr; IFADDRS = IFADDRS->ifa_next)
//    {
//        if (IFADDRS->ifa_addr == nullptr)
//        {
//            continue;
//        }
//        if (IFADDRS->ifa_addr->sa_family == AF_INET)
//        {
//            auto *IPV4 = (struct sockaddr_in *) IFADDRS->ifa_addr;
//            inet_ntop(AF_INET, &IPV4->sin_addr, IPSTR, sizeof IPSTR);
//            RUI_D_LOG_INFO("UDP服务端 IP:%-15s 端口:12345", IPSTR);
//        }
//    }
//
//    freeifaddrs(IFADDRS);

    int8_t YU_V_MOTOR_TYPE = 0;

    //收发数据
    while (1)
    {
        // 接收,里面解算没看懂，先按照我的思路写
        if (recvfrom(YU_U_SERVER_FD,YU_U_RECV.ALL,sizeof (YU_U_RECV.ALL),0,(struct sockaddr *)&YU_U_CLIENT_ADDR,&YU_U_CLIENT_ADDR_LEN) > 0)
        {
            // UDP 解算
        }

        // 发送,TM看不懂
        memcpy(&YU_U_SEND.PACKAGE.YU_V_DEBUG,&YU_V_DEBUG[YU_V_MOTOR_TYPE],sizeof (YU_TYPEDEF_DEBUG));

        sendto(YU_U_SERVER_FD,YU_U_SEND.ALL,sizeof (YU_U_SEND.ALL),0,(struct sockaddr *)&YU_U_CLIENT_ADDR,YU_U_CLIENT_ADDR_LEN);
        usleep(1);
    }


}
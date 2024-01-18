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
    struct sockaddr_in YU_V_SERVER_ADDR{},YU_V_CLIENT_ADDR;
    socklen_t YU_V_CLIENT_ADDR_LEN = sizeof (YU_V_CLIENT_ADDR);

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

    YU_TYPEDEF_SEND_UNION YU_V_SEND{ };
}
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
// 经过我不懈努力，终于能看懂部分了，先写写

static void YU_F_SET_A_P(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_A.IN.KP = value;}
static void YU_F_SET_A_I(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_A.IN.KI = value;}
static void YU_F_SET_A_D(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_A.IN.KD = value;}
static void YU_F_SET_A_ILIT(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_A.IN.I_LIT = value;}
static void YU_F_SET_A_ALIT(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_A.IN.ALL_LIT = value;}

static void YU_F_SET_S_P(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_S.IN.KP = value;}
static void YU_F_SET_S_I(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_S.IN.KI = value;}
static void YU_F_SET_S_D(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_S.IN.KD = value;}
static void YU_F_SET_S_ILIT(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_S.IN.I_LIT = value;}
static void YU_F_SET_S_ALIT(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_S.IN.ALL_LIT = value;}

static void YU_F_SET_C_P(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_C.IN.KP = value;}
static void YU_F_SET_C_I(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_C.IN.KI = value;}
static void YU_F_SET_C_D(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_C.IN.KD = value;}
static void YU_F_SET_C_ILIT(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_C.IN.I_LIT = value;}
static void YU_F_SET_C_ALIT(YU_TYPEDEF_DEBUG &MOTOR, const float &value) { MOTOR.PID_C.IN.ALL_LIT = value;}


static void YU_F_RECEIVE_SOLVE(const char *NAME, const float *DATA, int8_t MOTOR_TYPE)
{
    static const std::unordered_map<std::string, void (*)(YU_TYPEDEF_DEBUG &, const float &)> SETTER_MAP = {
            {"PID_A_P", YU_F_SET_A_P},
            {"PID_A_I", YU_F_SET_A_I},
            {"PID_A_D", YU_F_SET_A_D},
            {"A_I_LIT", YU_F_SET_A_ILIT},
            {"A_A_LIT", YU_F_SET_A_ALIT},

            {"PID_S_P", YU_F_SET_S_P},
            {"PID_S_I", YU_F_SET_S_I},
            {"PID_S_D", YU_F_SET_S_D},
            {"S_I_LIT", YU_F_SET_S_ILIT},
            {"S_A_LIT", YU_F_SET_S_ALIT},

            {"PID_C_P", YU_F_SET_C_P},
            {"PID_C_I", YU_F_SET_C_I},
            {"PID_C_D", YU_F_SET_C_D},
            {"C_I_LIT", YU_F_SET_C_ILIT},
            {"C_A_ALIT", YU_F_SET_C_ALIT},

    };

    auto IT = SETTER_MAP.find(std::string(NAME,7));
    if (IT != SETTER_MAP.end())
    {
        IT->second(YU_V_DEBUG[MOTOR_TYPE],*DATA);
    }
}


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

    struct ifaddrs * ifa;
    for(ifa = IFADDRS; ifa!= nullptr; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family != AF_INET)
            continue;
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            auto *IPV4 = (struct sockaddr_in *)ifa->ifa_addr;
            inet_ntop(AF_INET,&IPV4->sin_addr,IPSTR,sizeof (IPSTR));
            printf("Interface: %s\t Address: %s\n", ifa->ifa_name, IPSTR);
        }
    }

    int8_t YU_V_MOTOR_TYPE = 0;

    //收发数据
    while (1)
    {
        // 接收,里面解算没看懂，先按照我的思路写
        if (recvfrom(YU_U_SERVER_FD,YU_U_RECV.ALL,sizeof (YU_U_RECV.ALL),0,(struct sockaddr *)&YU_U_CLIENT_ADDR,&YU_U_CLIENT_ADDR_LEN) > 0)
        {
            // UDP 解算

            char CLIENT_IP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &YU_U_CLIENT_ADDR.sin_addr, CLIENT_IP, INET_ADDRSTRLEN);
            if (strncmp(YU_U_RECV.PACKAGE.NAME, "_MOTOR_", 7) == 0)
            {
                YU_V_MOTOR_TYPE = (int8_t) strtof((char *) &YU_U_RECV.PACKAGE.DATA, nullptr);
            }
            auto YU_V_TEMP_RE_DATA = strtof((char *) &YU_U_RECV.PACKAGE.DATA, nullptr);
            YU_F_RECEIVE_SOLVE((const char *) &YU_U_RECV.PACKAGE.NAME, &YU_V_TEMP_RE_DATA, YU_V_MOTOR_TYPE);

        }

        // 发送,TM看懂
        memcpy(&YU_U_SEND.PACKAGE.YU_V_DEBUG,&YU_V_DEBUG[YU_V_MOTOR_TYPE],sizeof (YU_TYPEDEF_DEBUG));

        sendto(YU_U_SERVER_FD,YU_U_SEND.ALL,sizeof (YU_U_SEND.ALL),0,(struct sockaddr *)&YU_U_CLIENT_ADDR,YU_U_CLIENT_ADDR_LEN);
        usleep(1);
    }


}
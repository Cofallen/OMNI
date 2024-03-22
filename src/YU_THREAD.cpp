//
// Created by 17273 on 2024/3/12.
//

#include "YU_PID.h"
#include "YU_CHASSIS.h"
#include "YU_MOTOR.h"
#include "YU_CAN.h"
#include "YU_UART.h"
#include "YU_THREAD.h"
#include "YU_GIMBAL.h"
#include "YU_DEBUG.h"
#include "YU_DEFINE.h"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <unordered_map>

//#define YU_DEBUG_CHASSIS 1

/**                    全局变量定义                    **/
YU_TYPEDEF_DBUS YU_V_DBUS;      // 遥控数据

YU_TYPEDEF_MOTOR MOTOR[10];     // 测试电机数据，用第4个 0x205
YU_TYPEDEF_TOP GM6020_TOP;      // 测试电机

YU_TYPEDEF_MONITOR_DBUS YU_V_MONITOR_DBUS;

YU_TYPEDEF_MOTOR YU_V_MOTOR_CHASSIS[4];
YU_TYPEDEF_TOP YU_V_TOP_DATA_CHASSIS { };

YU_TYPEDEF_MOTOR YU_V_MOTOR_GIMBAL[2] = {0};
YU_TYPEDEF_TOP YU_V_TOP_DATA_GIMBAL{ };

YU_TYPEDEF_DEBUG YU_V_DEBUG[10]{ };
int8_t MOTOR_TYPE = 9;

/**                    全局变量定义                    **/


[[noreturn]] void YU_F_THREAD_CHASSIS_MECANUM()
{
    const float YU_V_PID_CHASSIS[5] = {5,0.1,0,1000,3000};


    YU_T_PID_INIT(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_1].PID_S,YU_V_PID_CHASSIS);
    YU_T_PID_INIT(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_2].PID_S,YU_V_PID_CHASSIS);
    YU_T_PID_INIT(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_3].PID_S,YU_V_PID_CHASSIS);
    YU_T_PID_INIT(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_4].PID_S,YU_V_PID_CHASSIS);

    YU_F_CHASSIS_INIT();

    while (true)
    {
        usleep(1);

        YU_F_CAN_RECV(YU_V_MOTOR_CHASSIS, &YU_V_TOP_DATA_CHASSIS, 0);
#ifdef YU_DEBUG_CHASSIS
        printf("AMOTOR1:  %d  AMOTOR2:  %d\nAMOTOR3:  %d  AMOTOR4:  %d\n",
               YU_V_MOTOR_CHASSIS[0].DATA.ANGLE_NOW,
               YU_V_MOTOR_CHASSIS[1].DATA.ANGLE_NOW,
               YU_V_MOTOR_CHASSIS[2].DATA.ANGLE_NOW,
               YU_V_MOTOR_CHASSIS[3].DATA.ANGLE_NOW
        );
#endif
        YU_F_CHASSIS_MECANUM(&YU_V_DBUS, YU_D_MOD_CHASSIS);
        YU_F_CHASSIS_MECANUM_SEND(YU_V_MOTOR_CHASSIS);
    }

}

[[noreturn]] void YU_F_THREAD_TEST()
{
    while (true)
    {
        YU_F_CAN_RECV(MOTOR,&GM6020_TOP ,0);
        printf("ANGLE:%d\n",MOTOR[4].DATA.ANGLE_NOW);
        YU_F_CAN_SEND(0,0x1FF,1000,1000,1000,100);
        usleep(1);
    }

}


[[noreturn]] void YU_F_THREAD_MONITOR()
{
    YU_V_MONITOR_DBUS.STATUS = YU_D_MONITOR_DBUS_OFFLINE;
    YU_V_MONITOR_DBUS.TIME = 200;

    while (true)
    {
        usleep(1);

        YU_V_MONITOR_DBUS.TIME++;
        if (YU_V_MONITOR_DBUS.TIME >= 100)
        {
            YU_V_MONITOR_DBUS.STATUS = YU_D_MONITOR_DBUS_OFFLINE;
        } else
        {
            YU_V_MONITOR_DBUS.STATUS = YU_D_MONITOR_DBUS_ONLINE;
        }

        if (YU_V_MONITOR_DBUS.STATUS == YU_D_MONITOR_DBUS_OFFLINE)
        {
            memset(&YU_V_DBUS, 0, sizeof (YU_V_DBUS));
            printf("DBUS OFFLINE\n");
        }
    }
}

[[noreturn]] void YU_F_THREAD_GIMBAL()
{
    const float PID_GIMBAL_YAW_A[5] = {2.0f, 0, 0, 1000, 3000};
    const float PID_GIMBAL_YAW_S[5] = {2.0f, 0, 0, 1000, 3000};
    const float PID_GIMBAL_PIT_A[5] = {2.0f, 0, 0, 1000, 3000};
    const float PID_GIMBAL_PIT_S[5] = {2.0f, 0, 0, 1000, 3000};


    YU_T_PID_INIT(&YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_YAW].PID_S, PID_GIMBAL_YAW_S);
    YU_T_PID_INIT(&YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_YAW].PID_A, PID_GIMBAL_YAW_A);
    YU_T_PID_INIT(&YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_PIT].PID_S, PID_GIMBAL_PIT_S);
    YU_T_PID_INIT(&YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_PIT].PID_A, PID_GIMBAL_PIT_A);

    while (true)
    {
        usleep(1);

        YU_F_CAN_RECV(YU_V_MOTOR_GIMBAL,&YU_V_TOP_DATA_GIMBAL,1);

        YU_F_GIMBAL(&YU_V_DBUS, YU_D_MOD_GIMBAL);

        YU_F_GIMBAL_SEND(YU_V_MOTOR_GIMBAL);

    }
}


[[noreturn]] void YU_F_THREAD_VOFA()
{
    YU_TYPEDEF_SEND_UNION YU_U_SEND{ };
    YU_TYPEDEF_RECV_UNION YU_U_RECV{ };
    YU_U_SEND.DATA.TAIL[2] = 0x80;
    YU_U_SEND.DATA.TAIL[3] = 0x7f;

    struct sockaddr_in YU_U_SERVER_ADDR { }, YU_U_CLIENT_ADDR{ };
    socklen_t YU_U_CLIENT_ADDR_LEN = sizeof (YU_U_CLIENT_ADDR);
    struct ifaddrs *IFADDRS;
    char IPSTR[INET_ADDRSTRLEN];

    int YU_U_SOCKET_FD = socket(AF_INET, SOCK_DGRAM, 0);
    if (YU_U_SOCKET_FD == -1)
    {
        printf("SOCKET FD ERROR!\n");
        exit(1);
    }

    // 设置为非阻塞模式
    int FLAGS = fcntl(YU_U_SOCKET_FD,F_GETFL,0);
    if (fcntl(YU_U_SOCKET_FD,F_SETFL,FLAGS | O_NONBLOCK) == -1)
    {
        perror("fcntl error\n");
        exit(-1);
    }

    memset(&YU_U_SERVER_ADDR, 0, sizeof (YU_U_SERVER_ADDR));
    YU_U_SERVER_ADDR.sin_family = AF_INET;
    YU_U_SERVER_ADDR.sin_port = htons(12345);
    YU_U_SERVER_ADDR.sin_addr.s_addr = htonl(INADDR_ANY);
//    YU_U_SERVER_ADDR.sin_addr.s_addr = htonl(inet_addr("192.168.28.76"));
    if (bind(YU_U_SOCKET_FD, (sockaddr *)&YU_U_SERVER_ADDR, (socklen_t)sizeof (YU_U_SERVER_ADDR)) < 0)
    {
        perror("bind error!\n");
        exit(-1);
    }

    if (getifaddrs(&IFADDRS) == -1)
    {
        perror("ip 获取 error!\n");
        exit(-1);
    }

    struct ifaddrs *ifa;
    for (ifa = IFADDRS; ifa != nullptr; ifa = ifa->ifa_next)
    {
        if (ifa->ifa_addr == nullptr || ifa->ifa_addr->sa_family != AF_INET)
            continue;
        if (ifa->ifa_addr->sa_family == AF_INET)
        {
            auto *IPV4 = (struct sockaddr_in *)ifa->ifa_addr;
            inet_ntop(AF_INET, &IPV4->sin_addr, IPSTR, (socklen_t)sizeof (IPSTR));
            printf("Interface: %s\t Address: %s\n", ifa->ifa_name, IPSTR);
        }
    }



    while (true)
    {
        YU_F_VOFA_DEBUG();

        if (recvfrom(YU_U_SOCKET_FD,&YU_U_RECV,sizeof (YU_U_RECV.ALL), 0, (struct sockaddr *)&YU_U_CLIENT_ADDR, &YU_U_CLIENT_ADDR_LEN) > 0)
        {
            char CLIENT_IP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &YU_U_CLIENT_ADDR.sin_addr, CLIENT_IP, INET_ADDRSTRLEN);

            YU_F_VOFA_PARSE(&YU_U_RECV);

            YU_F_VOFA_ASSIGN(&YU_U_RECV);

//            MOTOR_TYPE = (int8_t)(YU_U_RECV.DATA.NAME);

        }

        memcpy(&YU_U_SEND.DATA.YU_V_MOTOR_DEBUG, &YU_V_DEBUG[MOTOR_TYPE],sizeof (YU_V_DEBUG[MOTOR_TYPE]));

        if (sendto(YU_U_SOCKET_FD, &YU_U_SEND, sizeof (YU_U_SEND.ALL), 0, (struct sockaddr *)&YU_U_CLIENT_ADDR, YU_U_CLIENT_ADDR_LEN) > 0)
        {
//            printf("ANGLE: %f\n",YU_U_SEND.DATA.YU_V_MOTOR_DEBUG.MOTOR_DATA.ANGLE);
//            printf("KP: %f\n",YU_U_SEND.DATA.YU_V_MOTOR_DEBUG.PID_S.IN.KP);
//            printf("MOTOR_DATA ANGLE:  %hd\n", YU_V_MOTOR_CHASSIS[0].DATA.ANGLE_NOW);
        }

        usleep(1);
    }
}

void YU_F_VOFA_PARSE(YU_TYPEDEF_RECV_UNION *RECV)
{
    std::string ITER = RECV->ALL;
    auto POS = ITER.find(':');  // 0x3A
    auto NAME_RAW= ITER.substr(0, POS);
    auto VALUE_RAW = ITER.substr(POS + 1);
    auto PARAM = std::stof(VALUE_RAW);

    strcpy(RECV->DATA.NAME,NAME_RAW.c_str());
    RECV->DATA.PARAM = PARAM;

//    printf("NAME = %s  PARAM = %f\n",RECV->DATA.NAME, RECV->DATA.PARAM);

}


/**
 * @details 简单赋值，将MOTOR数据赋给单个 DEBUG
 * @details 其他方法TODO : map映射 | 共用地址
 * @param DEBUG
 * @param MOTOR
 * @author YU
 * @date 2024-03-21
 */
void YU_F_VOFA_DEBUG_CAL(YU_TYPEDEF_DEBUG *DEBUG, YU_TYPEDEF_MOTOR *MOTOR)
{
    DEBUG->MOTOR_DATA.AIM = MOTOR->DATA.AIM;
    DEBUG->MOTOR_DATA.ANGLE = MOTOR->DATA.ANGLE_NOW;
    DEBUG->MOTOR_DATA.LAPS = MOTOR->DATA.LAPS;
    DEBUG->MOTOR_DATA.CURRENT = MOTOR->DATA.CURRENT;
    DEBUG->MOTOR_DATA.SPEED = MOTOR->DATA.SPEED_NOW;

    DEBUG->PID_S.OUT.ERROR[YU_D_NOW] = MOTOR->PID_S.OUT.ERROR[YU_D_NOW];
    DEBUG->PID_S.OUT.ERROR[YU_D_LAST] = MOTOR->PID_S.OUT.ERROR[YU_D_LAST];
    DEBUG->PID_S.OUT.P_OUT = MOTOR->PID_S.OUT.P_OUT;
    DEBUG->PID_S.OUT.I_OUT = MOTOR->PID_S.OUT.I_OUT;
    DEBUG->PID_S.OUT.D_OUT = MOTOR->PID_S.OUT.D_OUT;
    DEBUG->PID_S.OUT.ALL_OUT = MOTOR->PID_S.OUT.ALL_OUT;
    DEBUG->PID_S.IN.KP = MOTOR->PID_S.IN.KP;
    DEBUG->PID_S.IN.KI = MOTOR->PID_S.IN.KI;
    DEBUG->PID_S.IN.KD = MOTOR->PID_S.IN.KD;
    DEBUG->PID_S.IN.I_LIT = MOTOR->PID_S.IN.I_LIT;
    DEBUG->PID_S.IN.ALL_LIT = MOTOR->PID_S.IN.ALL_LIT;

    DEBUG->PID_A.OUT.ERROR[YU_D_NOW] = MOTOR->PID_A.OUT.ERROR[YU_D_NOW];
    DEBUG->PID_A.OUT.ERROR[YU_D_LAST] = MOTOR->PID_A.OUT.ERROR[YU_D_LAST];
    DEBUG->PID_A.OUT.P_OUT = MOTOR->PID_A.OUT.P_OUT;
    DEBUG->PID_A.OUT.I_OUT = MOTOR->PID_A.OUT.I_OUT;
    DEBUG->PID_A.OUT.D_OUT = MOTOR->PID_A.OUT.D_OUT;
    DEBUG->PID_A.OUT.ALL_OUT = MOTOR->PID_A.OUT.ALL_OUT;
    DEBUG->PID_A.IN.KP = MOTOR->PID_A.IN.KP;
    DEBUG->PID_A.IN.KI = MOTOR->PID_A.IN.KI;
    DEBUG->PID_A.IN.KD = MOTOR->PID_A.IN.KD;
    DEBUG->PID_A.IN.I_LIT = MOTOR->PID_A.IN.I_LIT;
    DEBUG->PID_A.IN.ALL_LIT = MOTOR->PID_A.IN.ALL_LIT;

    DEBUG->PID_C.OUT.ERROR[YU_D_NOW] = MOTOR->PID_C.OUT.ERROR[YU_D_NOW];
    DEBUG->PID_C.OUT.ERROR[YU_D_LAST] = MOTOR->PID_C.OUT.ERROR[YU_D_LAST];
    DEBUG->PID_C.OUT.P_OUT = MOTOR->PID_C.OUT.P_OUT;
    DEBUG->PID_C.OUT.I_OUT = MOTOR->PID_C.OUT.I_OUT;
    DEBUG->PID_C.OUT.D_OUT = MOTOR->PID_C.OUT.D_OUT;
    DEBUG->PID_C.OUT.ALL_OUT = MOTOR->PID_C.OUT.ALL_OUT;
    DEBUG->PID_C.IN.KP = MOTOR->PID_C.IN.KP;
    DEBUG->PID_C.IN.KI = MOTOR->PID_C.IN.KI;
    DEBUG->PID_C.IN.KD = MOTOR->PID_C.IN.KD;
    DEBUG->PID_C.IN.I_LIT = MOTOR->PID_C.IN.I_LIT;
    DEBUG->PID_C.IN.ALL_LIT = MOTOR->PID_C.IN.ALL_LIT;


}

void YU_F_VOFA_DEBUG()
{
    YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_C1], &YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_1]);
    YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_C2], &YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_2]);
    YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_C3], &YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_3]);
    YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_C4], &YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_4]);

    YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_GY], &YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_YAW]);
    YU_F_VOFA_DEBUG_CAL(&YU_V_DEBUG[YU_D_VOFA_GP], &YU_V_MOTOR_GIMBAL[YU_D_MOTOR_GIMBAL_PIT]);

}

void YU_F_VOFA_ASSIGN(YU_TYPEDEF_RECV_UNION *RECV)
{
    std::string OUTLINE = RECV->DATA.NAME;
    auto POS_1 = OUTLINE.find('_');             // HEAD_TAIL
    auto HEAD = OUTLINE.substr(0, POS_1);  //   C1_S_P
    auto TAIL = OUTLINE.substr(POS_1+1);

    // C1 C2 C3 C4 GY GP  // A S C  // P I D LIT
    static const std::unordered_map<std::string, int> MOTOR_MAP{
            {"C1", YU_D_VOFA_C1},
            {"C2", YU_D_VOFA_C2},
            {"C3", YU_D_VOFA_C3},
            {"C4", YU_D_VOFA_C4},
            {"GY", YU_D_VOFA_GY},
            {"GP", YU_D_VOFA_GP}
    };
    auto IT = MOTOR_MAP.find(HEAD);
    if (IT != MOTOR_MAP.end())
    {
        MOTOR_TYPE = IT->second;
    }

    printf("MOTOR_TYPE:  %d\n",MOTOR_TYPE);

//    static const std::unordered_map<std::string, >
//
}

void YU_F_VOFA_WRITE(YU_TYPEDEF_MOTOR *MOTOR, float PARAM)
{
//    static void
}
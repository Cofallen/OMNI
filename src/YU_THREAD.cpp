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

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <ifaddrs.h>

//#define YU_DEBUG_CHASSIS 1

/**                    全局变量定义                    **/
YU_TYPEDEF_DBUS YU_V_DBUS;      // 遥控数据

YU_TYPEDEF_MOTOR MOTOR[10];     // 测试电机数据，用第4个 0x205
YU_TYPEDEF_TOP GM6020_TOP;      // 测试电机

YU_TYPEDEF_MONITOR_DBUS YU_V_MONITOR_DBUS;

/**                    全局变量定义                    **/


[[noreturn]] void YU_F_THREAD_CHASSIS_MECANUM()
{
    const float YU_V_PID_CHASSIS[5] = {5,0.1,0,1000,3000};
    YU_TYPEDEF_MOTOR YU_V_MOTOR_CHASSIS[4];
    YU_TYPEDEF_TOP YU_V_TOP_DATA_CHASSIS { };

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
    YU_TYPEDEF_MOTOR YU_V_MOTOR_GIMBAL[2] = {0};
    YU_TYPEDEF_TOP YU_V_TOP_DATA_GIMBAL{ };

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
        if (recvfrom(YU_U_SOCKET_FD,&YU_U_RECV,sizeof (YU_U_RECV), 0, (struct sockaddr *)&YU_U_CLIENT_ADDR, &YU_U_CLIENT_ADDR_LEN) > 0)
        {

        }
    }
}
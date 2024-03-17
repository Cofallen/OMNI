//
// Created by 17273 on 2024/3/12.
//

#include "YU_PID.h"
#include "YU_CHASSIS.h"
#include "YU_MOTOR.h"
#include "YU_CAN.h"
#include "YU_UART.h"
#include "YU_THREAD.h"

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
    YU_TYPEDEF_TOP YU_V_TOP_DATA { };

    YU_T_PID_INIT(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_1].PID_S,YU_V_PID_CHASSIS);
    YU_T_PID_INIT(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_2].PID_S,YU_V_PID_CHASSIS);
    YU_T_PID_INIT(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_3].PID_S,YU_V_PID_CHASSIS);
    YU_T_PID_INIT(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_4].PID_S,YU_V_PID_CHASSIS);

    YU_F_CHASSIS_INIT();

    while (true)
    {
        YU_F_CAN_RECV(YU_V_MOTOR_CHASSIS, &YU_V_TOP_DATA, 0);
#ifdef YU_DEBUG_CHASSIS
        printf("AMOTOR1:  %d  AMOTOR2:  %d\nAMOTOR3:  %d  AMOTOR4:  %d\n",
               YU_V_MOTOR_CHASSIS[0].DATA.ANGLE_NOW,
               YU_V_MOTOR_CHASSIS[1].DATA.ANGLE_NOW,
               YU_V_MOTOR_CHASSIS[2].DATA.ANGLE_NOW,
               YU_V_MOTOR_CHASSIS[3].DATA.ANGLE_NOW
        );
#endif
        YU_F_CHASSIS_MECANUM(&YU_V_DBUS);
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
    YU_V_MONITOR_DBUS.TIME = 0;

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
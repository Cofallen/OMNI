//
// Created by 17273 on 2024/3/12.
//

#include "YU_PID.h"
#include "YU_CHASSIS.h"
#include "YU_MOTOR.h"
#include "YU_CAN.h"
#include "YU_UART.h"
#include "YU_THREAD.h"

/**                    全局变量定义                    **/
YU_TYPEDEF_DBUS YU_V_DBUS;      // 遥控数据

YU_TYPEDEF_MOTOR MOTOR[10];     // 测试电机数据，用第4个 0x205
YU_TYPEDEF_TOP GM6020_TOP;      // 测试电机
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
        YU_F_CAN_RECV(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_1], &YU_V_TOP_DATA, 0);
        YU_F_CAN_RECV(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_2], &YU_V_TOP_DATA, 0);
        YU_F_CAN_RECV(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_3], &YU_V_TOP_DATA, 0);
        YU_F_CAN_RECV(&YU_V_MOTOR_CHASSIS[YU_D_MOTOR_CHASSIS_4], &YU_V_TOP_DATA, 0);

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
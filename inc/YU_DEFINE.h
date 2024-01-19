/**
 * @brief 仿照RUI的命名规则写的，参数与RUI的完全一样
 * @author SiKeyu
 * @details 先这样用，以后会再封装一下
 * @version 0.1
 * @date 2024-01-18
 */



#ifndef DEMO_YU_DEFINE_H
#define DEMO_YU_DEFINE_H


/*****************************离线检测**********************************/
//需要离线检测的设备数量
#define YU_D_STATUS_NUM               11
//离线检测设备的ID序号
#define YU_D_STATUS_ID_CHASSIS_MOTOR_1 0
#define YU_D_STATUS_ID_CHASSIS_MOTOR_2 1
#define YU_D_STATUS_ID_CHASSIS_MOTOR_3 2
#define YU_D_STATUS_ID_CHASSIS_MOTOR_4 3
#define YU_D_STATUS_ID_GIMBAL_PIT     4
#define YU_D_STATUS_ID_GIMBAL_YAW     5
#define YU_D_STATUS_ID_ATTACK_L       6
#define YU_D_STATUS_ID_ATTACK_R       7
#define YU_D_STATUS_ID_ATTACK_G       8
#define YU_D_STATUS_ID_TOP            9
#define YU_D_STATUS_ID_DBUS           10
//设备状态
#define YU_D_STATUS_ON                1
#define YU_D_STATUS_OFF               2
#define YU_D_STATUS_OFF_ON            3
#define YU_D_STATUS_ON_OFF            4
//电机离线检测时间
#define YU_D_STATUS_JUDGE_TIME_MOTOR  200
//陀螺仪离线检测时间
#define YU_D_STATUS_JUDGE_TIME_TOP 15
/*****************************离线检测**********************************/



/******************************CAN_ID********************************/
#define YU_D_CAN_1 0
#define YU_D_CAN_2 1
//底盘
#define YU_D_CAN_ID_CHASSIS_1    0x201
#define YU_D_CAN_ID_CHASSIS_2    0x202
#define YU_D_CAN_ID_CHASSIS_3    0x203
#define YU_D_CAN_ID_CHASSIS_4    0x204
//电容
#define YU_D_CAN_ID_CAP          0x206
//陀螺仪
#define YU_D_CAN_ID_TOP         0x101
//云台
#define YU_D_CAN_ID_YAW         0x209
#define YU_D_CAN_ID_PIT         0x208
//发射
#define YU_D_CAN_ID_ATTACK_L    0x203
#define YU_D_CAN_ID_ATTACK_R    0x204
#define YU_D_CAN_ID_ATTACK_G    0x205
/******************************CAN_ID********************************/


/*****************************电机数组********************************/
#define YU_D_MOTOR_NUMBER        10
//电机序号
//底盘电机
#define YU_D_MOTOR_CHASSIS_1      0
#define YU_D_MOTOR_CHASSIS_2      1
#define YU_D_MOTOR_CHASSIS_3      2
#define YU_D_MOTOR_CHASSIS_4      3
//云台电机
#define YU_D_MOTOR_GIMBAL_YAW    4
#define YU_D_MOTOR_GIMBAL_PIT    3
#define YU_D_MOTOR_ATTACK_L      1
#define YU_D_MOTOR_ATTACK_R      0
#define YU_D_MOTOR_ATTACK_G      2
//电机类型
#define YU_D_MOTOR_TYPE_3508     1
#define YU_D_MOTOR_TYPE_3510     2
#define YU_D_MOTOR_TYPE_6020     3
#define YU_D_MOTOR_TYPE_2006     4
/****************************电机数组*********************************/

#endif //DEMO_YU_DEFINE_H

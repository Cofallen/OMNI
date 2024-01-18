/**
 * @brief 仿照RUI的命名规则写的，参数与RUI的完全一样
 * @author SiKeyu
 * @details 先这样用，以后会再封装一下
 * @version 0.1
 * @date 2024-01-18
 */


#ifndef DEMO_YU_DEFINE_H
#define DEMO_YU_DEFINE_H

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

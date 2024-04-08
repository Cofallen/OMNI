#include "YU_CAN.h"
#include <sys/select.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/can.h>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "YU_MOTOR.h"
#include "YU_TOP.h"
#include "YU_DEFINE.h"

struct can_frame frame[2];
int YU_C_FD[2];

// 非阻塞模式设定先不写

/**
 * @brief CAN初始化
 * @return 1 真值
 */
bool YU_F_CAN_INIT(void)
{
    printf("开始CAN初始化......\n");

    // 变量
    struct sockaddr_can YU_C_ADDR[2];
    struct ifreq YU_C_IFR[2];
    char *YU_C_IFNAMES[2] = {(char *)"can0",(char *)"can1"}; // 两个CAN接口名

    // 创建socket and bind can

    // CAN1
    if ((YU_C_FD[0] = socket(PF_CAN,SOCK_RAW,CAN_RAW))>0)
    {
        printf("CAN FD build OK\n");
    } else
    {
        perror("CAN FD build failure\n");
//        exit(1);
    }

    // 这里RUI直接访问.ifrn_name，我感觉不太好，有空再看看
    // RUI 用的宏定义。。。 挺好，我也用（才怪）。
    //  # define ifr_name	ifr_ifrn.ifrn_name	/* interface name 	*/
    strcpy(YU_C_IFR[0].ifr_ifrn.ifrn_name,YU_C_IFNAMES[0]);

    if(ioctl(YU_C_FD[0],SIOCGIFINDEX,&YU_C_IFR[0]) < 0)
    {
        perror("Please check if CAN1 EXISTS \n");
        close(YU_C_FD[0]);
//        exit(-1);
    }

    YU_C_ADDR[0].can_family = AF_CAN;
    // # define ifr_ifindex	ifr_ifru.ifru_ivalue    /* interface index      */
    YU_C_ADDR[0].can_ifindex = YU_C_IFR[0].ifr_ifru.ifru_ivalue;

    if (bind(YU_C_FD[0],(struct sockaddr *)&YU_C_ADDR[0],(socklen_t )sizeof (YU_C_ADDR[0])) == 0)
    {
        printf("CAN1 bind OK\n");
    } else
    {
        perror("CAN1 bind error\n");
        close(YU_C_FD[0]);
//        exit(-1);
    }

    // CAN2
    if ((YU_C_FD[1] = socket(PF_CAN,SOCK_RAW,CAN_RAW))>0)
    {
        printf("CAN FD build OK\n");
    } else
    {
        perror("CAN FD build failure\n");
//        exit(-1);
    }

    strcpy(YU_C_IFR[1].ifr_ifrn.ifrn_name,YU_C_IFNAMES[1]);

    if(ioctl(YU_C_FD[1],SIOCGIFINDEX,&YU_C_IFR[1]) < 0)
    {
        perror("Please check if CAN2 EXISTS \n");
        close(YU_C_FD[1]);
//        exit(-1);
    }

    YU_C_ADDR[1].can_family = AF_CAN;
    YU_C_ADDR[1].can_ifindex = YU_C_IFR[1].ifr_ifru.ifru_ivalue;

    if (bind(YU_C_FD[1],(struct sockaddr *)&YU_C_ADDR[1],(socklen_t )sizeof (YU_C_ADDR[1])) == 0)
    {
        printf("CAN2 bind OK\n");
    } else
    {
        perror("CAN2 bind error\n");
        close(YU_C_FD[1]);
//        exit(-1);
    }

    return 1;

}


// CAN 解算，先写老版的吧，新版的看不懂。

/**
 * @brief CAN 解算
 * @param MOTOR 电机结构体指针
 * @param YU_C_TOP_DATA
 * @param can
 * @param CAN_ID CAN ID
 * @param CAN_DATA
 * @return
 */
static bool YU_F_CAN_RESOLVE(YU_TYPEDEF_MOTOR *MOTOR,YU_TYPEDEF_TOP *YU_V_TOP_DATA,uint8_t can,u_int32_t CAN_ID,uint8_t *CAN_DATA)
{
    if (can == YU_D_CAN_1)
    {
//        printf("CANID%x\n",CAN_ID);
        // 这几个解算也看不懂
        switch (CAN_ID)
        {
            case YU_D_CAN_ID_YAW: YU_F_MOTOR_CAN_CAL(&MOTOR[YU_D_MOTOR_GIMBAL_YAW],CAN_DATA,YU_D_MOTOR_TYPE_6020,YU_D_STATUS_ID_GIMBAL_YAW);
                break;
            case YU_D_CAN_ID_PIT: YU_F_MOTOR_CAN_CAL(&MOTOR[YU_D_MOTOR_GIMBAL_PIT], CAN_DATA, YU_D_MOTOR_TYPE_6020, YU_D_STATUS_ID_GIMBAL_PIT);
                break;
            case YU_D_CAN_ID_ATTACK_L:YU_F_MOTOR_CAN_CAL(&MOTOR[YU_D_MOTOR_ATTACK_L], CAN_DATA, YU_D_MOTOR_TYPE_3508, YU_D_STATUS_ID_ATTACK_L);
                break;
            case YU_D_CAN_ID_ATTACK_R:YU_F_MOTOR_CAN_CAL(&MOTOR[YU_D_MOTOR_ATTACK_R], CAN_DATA, YU_D_MOTOR_TYPE_3508, YU_D_STATUS_ID_ATTACK_R);
                break;
//            case YU_D_CAN_ID_ATTACK_G:YU_F_MOTOR_CAN_CAL(&MOTOR[YU_D_MOTOR_ATTACK_G], CAN_DATA, YU_D_MOTOR_TYPE_2006, YU_D_STATUS_ID_ATTACK_G);
//                break;
//            case YU_D_CAN_ID_TOP:YU_F_TOP_CAN_CAL(YU_C_TOP_DATA, CAN_DATA);
//                break;
            default:break;

        }
    }

//    else   // can == 1
//    {
        switch (CAN_ID)
        {
            case YU_D_CAN_ID_CHASSIS_1:YU_F_MOTOR_CAN_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_1], CAN_DATA, YU_D_MOTOR_TYPE_3508, YU_D_STATUS_ID_CHASSIS_MOTOR_1);
                break;
            case YU_D_CAN_ID_CHASSIS_2:YU_F_MOTOR_CAN_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_2], CAN_DATA, YU_D_MOTOR_TYPE_3508, YU_D_STATUS_ID_CHASSIS_MOTOR_2);
                break;
            case YU_D_CAN_ID_CHASSIS_3:YU_F_MOTOR_CAN_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_3], CAN_DATA, YU_D_MOTOR_TYPE_3508, YU_D_STATUS_ID_CHASSIS_MOTOR_3);
                break;
            case YU_D_CAN_ID_CHASSIS_4:YU_F_MOTOR_CAN_CAL(&MOTOR[YU_D_MOTOR_CHASSIS_4], CAN_DATA, YU_D_MOTOR_TYPE_3508, YU_D_STATUS_ID_CHASSIS_MOTOR_4);
                break;

            default:break;
        }
//    }

    return  true;
}

// CAN 接收先写旧版 有时间全看一遍，都不太懂

bool YU_F_CAN_RECV(YU_TYPEDEF_MOTOR *MOTOR,YU_TYPEDEF_TOP *YU_V_TOP_DATA,uint8_t can)
{
    ssize_t DATA_FLAG;
    fd_set YU_READ_FDS;
    struct timeval YU_CAN_TIMEOUT{ };

    FD_ZERO(&YU_READ_FDS);
    FD_SET(YU_C_FD[can],&YU_READ_FDS);

    YU_CAN_TIMEOUT.tv_sec = 1; // 设置超时时间为5s
    YU_CAN_TIMEOUT.tv_usec = 0;

    int SELECT_RESULT = select(YU_C_FD[can]+1,&YU_READ_FDS, nullptr, nullptr,&YU_CAN_TIMEOUT);

    if (SELECT_RESULT > 0)
    {
        // 收到数据
        DATA_FLAG = read(YU_C_FD[can],&frame[can],sizeof (struct can_frame));
        if (DATA_FLAG > 0)
        {
            YU_F_CAN_RESOLVE(MOTOR,YU_V_TOP_DATA,can,frame[can].can_id,frame[can].data);
            return 1;
        } else
        {
            return -1;
        }
    } else
    {
        return -1;
    }
}


// CAN 发送

 bool YU_F_CAN_SEND(uint8_t can,uint32_t can_id,int16_t num1,int16_t num2,int16_t num3,int16_t num4)
{
    struct can_frame CAN_FRAME{ };

    CAN_FRAME.can_id = can_id;
    CAN_FRAME.can_dlc = 0x08;

    // RUI 没有强制类型转换
    CAN_FRAME.data[0] = ((num1) >> 8);
    CAN_FRAME.data[1] = num1;
    CAN_FRAME.data[2] = ((num2) >> 8);
    CAN_FRAME.data[3] = (num2);
    CAN_FRAME.data[4] = ((num3) >> 8);
    CAN_FRAME.data[5] = (num3);
    CAN_FRAME.data[6] = ((num4) >> 8);
    CAN_FRAME.data[7] = (num4);


    write(YU_C_FD[can], &CAN_FRAME, sizeof(struct can_frame)) ;

    return 1;
}


// 电机CAN发送
// 哈哈哈哈哈哈哈哈哈 不写了--地盘
//
// Created by 17273 on 2024/1/15.
//

#ifndef DEMO_YU_CAN_H
#define DEMO_YU_CAN_H

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "YU_MOTOR.h"
#include "YU_TOP.h"
#include "YU_DEFINE.h"

extern struct can_frame frame[2];
extern int YU_C_FD[2];


bool YU_F_CAN_INIT(void);
static bool YU_F_CAN_RESOLVE(YU_TYPEDEF_MOTOR *MOTOR,YU_TYPEDEF_TOP *YU_C_TOP_DATA,uint8_t can,u_int32_t CAN_ID,uint8_t *CAN_DATA);
bool YU_F_CAN_RECV(YU_TYPEDEF_MOTOR *MOTOR,YU_TYPEDEF_TOP *YU_V_TOP_DATA,uint8_t can);
static bool YU_F_CAN_SEND(uint8_t can,u_int32_t can_id,int16_t num1,int16_t num2,int16_t num3,int16_t num4);

#endif //DEMO_YU_CAN_H

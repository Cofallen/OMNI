//
// Created by 17273 on 2024/4/24.
//

#ifndef DEMO_YU_VISION_H
#define DEMO_YU_VISION_H

#include <cstdio>
#include <cstdint>

union YU_TYPEDEF_VISION_SEND
{
    struct
    {
        float PIT_DATA;
        float YAW_DATA;
        float INIT_FIRE_SPEED;
        int FLAG;
        bool COLOR;
        unsigned int TIME;
    }DATA;
    uint8_t ALL[24];  // 标记，感觉是24
};
union YU_TYPEDEF_VISION_RECV
{
    struct
    {
        float PIT_DATA;
        float YAW_DATA;
        bool TARGET;
    }DATA;
    uint8_t ALL[9];
};

typedef struct
{
    union YU_TYPEDEF_VISION_RECV RECV;
    union YU_TYPEDEF_VISION_SEND SEND;
}YU_TYPEDEF_VISION;

extern YU_TYPEDEF_VISION YU_V_VISION;

[[noreturn]] void YU_F_THREAD_VISION(YU_TYPEDEF_VISION *VISION);


#endif //DEMO_YU_VISION_H

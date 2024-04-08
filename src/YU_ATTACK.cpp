//
// Created by 17273 on 2024/4/1.
//
#include <string>
#include "YU_ATTACK.h"
#include "YU_MOTOR.h"
#include "YU_DEFINE.h"
#include "YU_CONFIG.h"

struct
{
    int WIPE_MAX_SPEED;
    int WIPE_MIN_SPEED;
    int WIPE_MID_SPEED;
    int SINGLE_ANGLE;
} YU_TYPEDEF_ATTACK;

bool RUI_F_ATTACK_INIT()
{
    std::string FILENAME = YU_D_CONFIG_PATH;
    YU_TYPEDEF_PID_CONFIG CONFIG = YU_F_PARSE_CONFIG(FILENAME);
    YU_TYPEDEF_ATTACK.WIPE_MAX_SPEED = std::stoi(CONFIG[ "ATTACK" ][ "MAX_SPEED" ]);
    YU_TYPEDEF_ATTACK.WIPE_MIN_SPEED = std::stoi(CONFIG[ "ATTACK" ][ "MIN_SPEED" ]);
    YU_TYPEDEF_ATTACK.WIPE_MID_SPEED = std::stoi(CONFIG[ "ATTACK" ][ "MID_SPEED" ]);
    YU_TYPEDEF_ATTACK.SINGLE_ANGLE = std::stoi(CONFIG[ "ATTACK" ][ "SINGLE_ANGLE" ]);
    printf("ATTACK init OK\n");
    return true;
}


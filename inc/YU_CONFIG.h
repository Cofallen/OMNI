//
// Created by 17273 on 2024/2/27.
//

#ifndef DEMO_YU_CONFIG_H
#define DEMO_YU_CONFIG_H

#include "YU_DEFINE.h"
#include <fstream>
#include <map>
#include <algorithm>
#include <cstring>

using YU_TYPEDEF_PID_CONFIG = std::map<std::string,std::map<std::string,std::string>>;

YU_TYPEDEF_PID_CONFIG YU_F_PARSE_CONFIG(const std::string &YU_V_FILENAME);
void YU_F_WRITE_CONFIG(const YU_TYPEDEF_PID_CONFIG &YU_V_CONFIG , const std::string &YU_V_FILENAME);

// 解析配置文件函数

#endif //DEMO_YU_CONFIG_H

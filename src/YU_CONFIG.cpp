//
// Created by 17273 on 2024/2/27.
//

#include <iostream>
#include "YU_CONFIG.h"

// 解析函数
YU_TYPEDEF_PID_CONFIG YU_F_PARSE_CONFIG(const std::string &YU_V_FILENAME)
{
    std::ifstream YU_V_FILE(YU_V_FILENAME);
    YU_TYPEDEF_PID_CONFIG YU_V_CONFIG;
    std::string YU_V_LINE;
    std::string YU_V_CURRENT_SECTION;

    if (!YU_V_FILE.is_open())
    {
        perror("open config.ini FAILURE\n");
//        exit(-1);
    }

    while (getline(YU_V_FILE,YU_V_LINE)) {
        // 去除行尾的空白字符
        YU_V_LINE.erase(std::remove_if(YU_V_LINE.begin(), YU_V_LINE.end(),::isspace),YU_V_LINE.end());

        if (YU_V_LINE.empty() || YU_V_LINE[0] == ';')
        {
            // 忽略空行和注释
            continue;
        }
        if (YU_V_LINE[0] == '[') {
            // 新的部分
            auto YU_V_END = YU_V_LINE.find(']');
            if (YU_V_END != std::string::npos)
            {
                YU_V_CURRENT_SECTION = YU_V_LINE.substr(1, YU_V_END - 1);
            }
        } else
        {
            // 键值对
            auto YU_V_DELIMITER_POS = YU_V_LINE.find('=');
            auto YU_V_KEY = YU_V_LINE.substr(0, YU_V_DELIMITER_POS);
            auto YU_V_VALUE = YU_V_LINE.substr(YU_V_DELIMITER_POS + 1);
            YU_V_CONFIG[YU_V_CURRENT_SECTION][YU_V_KEY] = YU_V_VALUE;
        }
    }

    // 关闭文件
    YU_V_FILE.close();

    return YU_V_CONFIG;
}

void YU_F_WRITE_CONFIG(const YU_TYPEDEF_PID_CONFIG &YU_V_CONFIG , const std::string &YU_V_FILENAME)
{
    std::ofstream YU_V_FILE(YU_V_FILENAME);

    if (!YU_V_FILE.is_open())
    {
        perror("open config FAILURE\n");
        return;
    }

    for (const auto &section : YU_V_CONFIG)
    {
        YU_V_FILE << "[" << section.first << "]\n";
        for (const auto &KV : section.second)
        {
            YU_V_FILE << KV.first << "=" << KV.second << "]\n";
        }
    }

    YU_V_FILE.close();
}
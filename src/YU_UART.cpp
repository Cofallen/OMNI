//
// Created by 17273 on 2024/3/8.
//
#include "YU_UART.h"
#include "YU_CONFIG.h"
#include "stdio.h"
#include "YU_ROOT_INIT.h"
#include "YU_THREAD.h"
#include "YU_MATH.h"

#include <mutex>

// 这个有问题
//struct YU_TYPEDEF_MONITOR YU_V_MONITOR{ };

[[noreturn]] void YU_F_DBUS_THREAD(YU_TYPEDEF_DBUS *YU_V_DBUS)
{
    std::string FILENAME = YU_D_CONFIG_PATH;
    YU_TYPEDEF_PID_CONFIG CONFIG = YU_F_PARSE_CONFIG(FILENAME);

    // 矫正 数据
    static int8_t S_ARRAY_CHOSE[3][3] = {
            {-1, 0, 1 },
            {2, -1, 3 },
            {4, 5, -1 }
    };
    static uint8_t CORRECTION_ARRAY[6][3] = {
            {0, 2, 1 },
            {0, 1, 2 },
            {2, 0, 1 },
            {1, 0, 2 },
            {2, 1, 0 },
            {1, 2, 0 }
    };

    // 矫正 选择
//    static int8_t CORRECTION_CHOSE_L = (int8_t) std::stof(CONFIG["DBUS"]["S_L"]);
//    static int8_t CORRENTION_CHOSE_R = (int8_t) std::stoi(CONFIG["DBUS"]["S_R"]);

    int8_t UART_FD;

    printf("串口初始化\n");

    struct termios2 OPTIONS { };

    UART_FD = (int8_t) open(YU_D_UART_PATH, O_RDWR | O_NOCTTY | O_NDELAY);

    if (UART_FD > 0)
    {
        printf("串口打开 OK\n");
    } else
    {
        printf("串口打开 FALIURE\n");
        perror("WRONG UART OPEN\n");
        exit(-1);
    }
        OPTIONS.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP | INLCR | IGNCR | ICRNL | IXON);
        OPTIONS.c_iflag |= (INPCK | IGNPAR);
        OPTIONS.c_oflag &= ~OPOST;
        OPTIONS.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);
        OPTIONS.c_cflag &= ~(CSIZE | CRTSCTS | PARODD | CBAUD);
        OPTIONS.c_cflag |= (CS8 | CSTOPB | CLOCAL | PARENB | BOTHER | CREAD);
        OPTIONS.c_ispeed = 100000;
        OPTIONS.c_ospeed = 100000;
        OPTIONS.c_cc[VMIN] = 25;
        OPTIONS.c_cc[VTIME] = 0;
        //TCSETS2 和 struct termios2 非标准波特率
        //TCSETS  和 struct termios    标准波特率

    if (0 != ioctl(UART_FD, TCSETS2, &OPTIONS)) {
        close(UART_FD);

        printf("WRONG\n");
        perror("Please check if UART EXISTS\n");
        exit(-1);
    }

    printf("UART INIT OK\n");

    union YU_TYPEDEF_DBUS_UNION YU_V_DBUS_UNION{ };

    memset(YU_V_DBUS_UNION.GET_DATA, 0, sizeof (YU_V_DBUS_UNION.GET_DATA));

#ifdef YU_DEBUG_DBUS_RECTIFY
    // 确保能接收到遥控器数据后
    while (YU_V_DBUS->REMOTE.S1_u8 == 0 || YU_V_DBUS->REMOTE.S2_u8 == 0)
    {
        usleep(1);

        if (read(UART_FD, YU_V_DBUS_UNION.GET_DATA, sizeof (YU_V_DBUS_UNION.GET_DATA) > 0))
        {
            YU_V_DBUS->REMOTE.S1_u8 = YU_V_DBUS_UNION.DATA_NEATEN.S1;
            YU_V_DBUS->REMOTE.S2_u8 = YU_V_DBUS_UNION.DATA_NEATEN.S2;
        }
    }
#endif
    // 遥控器矫正先不写



    // 遥控器数据接收
    while (true)
    {
        {
            std::lock_guard<std::mutex> LOCK(YU_V_MONITOR_DBUS.MUTEX);

            usleep(1);
            auto DATA_FLAG =  read(UART_FD, YU_V_DBUS_UNION.GET_DATA,sizeof (YU_V_DBUS_UNION.GET_DATA));

//        if (DATA_FLAG > 0)
//            printf("DATA FLAG: %ld\n",DATA_FLAG);

            if (DATA_FLAG == 12)
            {
                YU_V_DBUS->REMOTE.S1_u8 = YU_V_DBUS_UNION.DATA_NEATEN.S1;
                YU_V_DBUS->REMOTE.S2_u8 = YU_V_DBUS_UNION.DATA_NEATEN.S2;

                YU_V_DBUS->REMOTE.CH0_int16 = (int16_t)(YU_V_DBUS_UNION.DATA_NEATEN.CH0 - 1024);
                YU_V_DBUS->REMOTE.CH1_int16 = (int16_t)(YU_V_DBUS_UNION.DATA_NEATEN.CH1 - 1024);
                YU_V_DBUS->REMOTE.CH2_int16 = (int16_t)(YU_V_DBUS_UNION.DATA_NEATEN.CH2 - 1024);
                YU_V_DBUS->REMOTE.CH3_int16 = (int16_t)(YU_V_DBUS_UNION.DATA_NEATEN.CH3 - 1024);
                YU_V_DBUS->REMOTE.DIR_int16 = (int16_t)(YU_V_DBUS_UNION.DATA_NEATEN.DIR - 1024);

                // 发射标志位
//            YU_V_DBUS->L_FLAG = CORRECTION_ARRAY[CORRECTION_CHOSE_L][YU_V_DBUS->REMOTE.S1_u8 - 1];
//            YU_V_DBUS->R_FLAG = CORRECTION_ARRAY[CORRENTION_CHOSE_R][YU_V_DBUS->REMOTE.S2_u8 - 1];
                YU_V_MONITOR_DBUS.STATUS = YU_D_MONITOR_ONLINE;
            }
        }

            if (YU_V_DBUS->REMOTE.CH0_int16 <= 10 && YU_V_DBUS->REMOTE.CH0_int16 >= -10) YU_V_DBUS->REMOTE.CH0_int16 = 0;
            if (YU_V_DBUS->REMOTE.CH1_int16 <= 10 && YU_V_DBUS->REMOTE.CH1_int16 >= -10) YU_V_DBUS->REMOTE.CH1_int16 = 0;
            if (YU_V_DBUS->REMOTE.CH2_int16 <= 10 && YU_V_DBUS->REMOTE.CH2_int16 >= -10) YU_V_DBUS->REMOTE.CH2_int16 = 0;
            if (YU_V_DBUS->REMOTE.CH3_int16 <= 10 && YU_V_DBUS->REMOTE.CH3_int16 >= -10) YU_V_DBUS->REMOTE.CH3_int16 = 0;

        YU_V_MONITOR_DBUS.CV.notify_one();


//            printf("ch0  %4d  ch1  %4d  ch2  %4d  ch3  %4d  s1  %d  s2  %d\n",
//                   YU_V_DBUS->REMOTE.CH0_int16, YU_V_DBUS->REMOTE.CH1_int16,
//                   YU_V_DBUS->REMOTE.CH2_int16, YU_V_DBUS->REMOTE.CH3_int16,
//                   YU_V_DBUS->REMOTE.S1_u8,     YU_V_DBUS->REMOTE.S2_u8);

//        memset(YU_V_DBUS, 0, sizeof (*YU_V_DBUS));

    }
}



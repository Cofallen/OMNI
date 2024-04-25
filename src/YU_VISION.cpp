//
// Created by 17273 on 2024/4/24.
//
#include "YU_VISION.h"
#include "YU_DEFINE.h"

#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include "stdio.h"
#include "string.h"
#include <thread>

YU_TYPEDEF_VISION YU_V_VISION;

// 待优化接收发送,扔进小循环
[[noreturn]] void YU_F_THREAD_VISION(YU_TYPEDEF_VISION *VISION)
{
    while (true)
    {
        static int SOCKET_FD;
        static struct sockaddr_in SERVER_ADDR{ }, CLIENT_ADDR{ };
        socklen_t CLIENT_ADDR_LEN = sizeof(struct sockaddr_in);

        SOCKET_FD = socket(AF_INET, SOCK_STREAM, 0);
        if (SOCKET_FD == -1)
        {
            perror("视觉SOCKET FD 创建失败\n");
        }

        memset(&SERVER_ADDR, 0, sizeof(SERVER_ADDR));
        SERVER_ADDR.sin_family = AF_INET;
        SERVER_ADDR.sin_port = htons(8080); // 随便给的
        SERVER_ADDR.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(SOCKET_FD,(struct sockaddr *)&SERVER_ADDR,(socklen_t)sizeof(SERVER_ADDR)) < 0)
        {
            perror("视觉 bind 失败\n");
        }
        if (listen(SOCKET_FD, 5) == -1)
        {
            perror("视觉 Listen 失败\n");
            close(SOCKET_FD);
        }

        // 接收，发送
        while (accept(SOCKET_FD,(struct sockaddr *)&CLIENT_ADDR,&CLIENT_ADDR_LEN) >= 0)
        {
            if (recv(SOCKET_FD, &VISION->RECV, sizeof(VISION->RECV.ALL), 0) < 0)
            {
                perror("从视觉 recv 失败\n");
                break;
            }
            if (send(SOCKET_FD, &VISION->SEND, sizeof(VISION->SEND.ALL), 0) < 0)
            {
                perror("从视觉 send 失败\n");
                break;
            }
            usleep(1);
        }

        perror("视觉 accept 失败\n");
        close(SOCKET_FD);
        printf("尝试重连......\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));  // 延时2s
    }
}
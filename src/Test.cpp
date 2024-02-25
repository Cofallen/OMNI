#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// 数据包
char RxMsg[1024] = {0};
char TxMsg[5] = {0, 1, 2, 3, 4};

// justfloat 结构体
union DATA_TYPEDEF_DEMO
{
    struct
    {
        float PID[10];
        unsigned char TAIL[4];
    }PACKAGE;
    char All[44];
};

float DATA_TEMP[10] = {0};

void YU_F_SOCKET_UDP_VOFA(void)
{
    //发送数据初始化
    DATA_TYPEDEF_DEMO DATA_SEND{};
//    for (int i = 0; i < 10; ++i) {
//        DATA_SEND.PACKAGE.PID[i] = (float)(i);
//    }
//    DATA_SEND.PACKAGE.PID[];
    DATA_SEND.PACKAGE.TAIL[2] = 0x80;
    DATA_SEND.PACKAGE.TAIL[3] = 0x7f;
    printf("数据初始化成功\n");

    int SERVER_FD = 0;
    struct sockaddr_in SERVER_ADDR = {}, CLIENT_ADDR = {};
    socklen_t CLIENT_ADDR_LEN = sizeof (CLIENT_ADDR);

    // 初始化
    memset(&SERVER_ADDR, 0, sizeof(SERVER_ADDR));
    SERVER_ADDR.sin_family = AF_INET;
    SERVER_ADDR.sin_port = htons(12345);
    SERVER_ADDR.sin_addr.s_addr = htonl(INADDR_ANY);
//    SERVER_ADDR.sin_addr.s_addr = inet_addr("119.178.43.143");

    // FD创建
    if (-1 == (SERVER_FD = socket(AF_INET, SOCK_DGRAM, 0)))
    {
        printf("FD失败\n");
        exit(1);
    }

    // bind
    if (bind(SERVER_FD, (struct sockaddr *)&SERVER_ADDR, sizeof(SERVER_ADDR)) < 0)
    {
        printf("bind error!\n");
        exit(1);
    }

    /* 接受客户端的返回数据 */
    int RxMsg_len;
    if ((RxMsg_len = recvfrom(SERVER_FD, RxMsg, sizeof(RxMsg), 0, (struct sockaddr *)&CLIENT_ADDR, &CLIENT_ADDR_LEN)) < 0)
    {
        perror("recvfrom failed\n");
        exit(1);
    }

    printf("客户端发送过来的数据为：%s\n", RxMsg);

    while (1)
    {
        /* 清空缓冲区 */
//         printf("请发送\n");

//         fgets((char *)TxMsg, sizeof(TxMsg), stdin);

//        /* 发送数据到客户端 */
//        if (sendto(SERVER_FD, TxMsg, 5, 0, (struct sockaddr *)&CLIENT_ADDR, CLIENT_ADDR_LEN) < 0)
//        {
//            perror("sendto failed");
//            exit(1);
//        }

//        if (sendto(SERVER_FD, &DATA_SEND, sizeof (DATA_SEND), 0, (struct sockaddr *)&CLIENT_ADDR, CLIENT_ADDR_LEN) < 0)
//        {
//            perror("sendto failed");
//            exit(1);
//        }

        for (int i = 0; i < 10; ++i) {
            printf("请输入第%d个数：\n",i);
            scanf("%f",&DATA_TEMP[i]);
            DATA_SEND.PACKAGE.PID[i] = DATA_TEMP[i];
        }

        if (sendto(SERVER_FD,&DATA_SEND,sizeof (DATA_SEND),0,(sockaddr *)&CLIENT_ADDR,CLIENT_ADDR_LEN)<0)
        {
            perror("sendto error\n");
            exit(1);
        }

        memset(DATA_TEMP,0,sizeof (DATA_TEMP));

        // memset(TxMsg, 0, sizeof(TxMsg));
    }
}

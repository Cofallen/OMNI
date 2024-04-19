# RUI的作业

## 日志

* 2024-01-20 今天基本完成RUI留的作业，距离回家还有两个小时，抓紧准备一下
* 2024-01-21 现在主要还是线程安排，socket CAN 的应用。不知道为什么测试线程没run。
* 2024-02-25 解决了VOFA不能用的问题
* 2024-03-02 `CONFIG`编译成功
* 2024-03-08 `PID`基本完成，初始化读取`CONFIG`还是有点迷糊
* 2024-04-10 can初始化需要分阶段插，找不同`id`，另外，不要用反向`2pin`线

## 问题

- [ ] `can`不同固件初始化问题：(有问题)

基本初始化命令:

```shell
sudo modprobe mttcan 
sudo ip link set can1 type can bitrate 1000000
sudo ip link set can1 up

sudo slcand -o -c -s8 /dev/ttyACM0 can2
sudo ifconfig can2 up
```
* 这里`can1->chassis`，`can2->gimbal`

不同的`can`设备固件可能不同，现在麦轮小车上老`can`是`slcan with FD support`,新`can`是`candlellight`

如果你要刷`can`，请按如下方式操作：
1. 在[CANable](https://canable.io/updater/canable2.html)官网上，找到红字`this utility`，点击安装驱动;
2. 将你的`can`设备的`boot`引脚短接后，记得重新上电;
3. 选择正确的协议，点击`Connect and Update`按钮，理论将完成刷`can`.

- [ ] `Docker` 问题

* 镜像太大，基于`ubuntu:20.04`，总内存达到了

```shell
REPOSITORY   TAG       IMAGE ID       CREATED         SIZE
omni         latest    133b659089e7   4 minutes ago   366MB
```

* 镜像构建时间过长：(待查看)

```shell
[+] Building 72.1s (11/11) FINISHED                                                                                                         docker:default
 => [internal] load build definition from Dockerfile                                                                                        0.0s
 => => transferring dockerfile: 214B                                                                                                        0.0s
 => [internal] load metadata for docker.io/library/ubuntu:20.04                                                                             1.9s
 => [internal] load .dockerignore                                                                                                           0.0s
 => => transferring context: 2B                                                                                                             0.0s
 => [1/6] FROM docker.io/library/ubuntu:20.04@sha256:71b82b8e734f5cd0b3533a16f40ca1271f28d87343972bb4cd6bd6c38f1bd38e                       0.0s
 => [internal] load build context                                                                                                           0.0s
 => => transferring context: 6.28kB                                                                                                         0.0s
 => CACHED [2/6] WORKDIR /YU                                                                                                                0.0s
 => [3/6] COPY . .                                                                                                                          0.1s
 => [4/6] RUN chmod +x start.sh                                                                                                             0.2s
 => [5/6] RUN apt-get update && apt-get install -y gcc    cmake                                                                             66.7s
 => [6/6] RUN ./start.sh                                                                                                                    0.4s 
 => exporting to image                                                                                                                      2.7s
 => => exporting layers                                                                                                                     2.7s
 => => writing image sha256:133b659089e75a33ae8c09c40afdb680fa7e009c8dda6a6da12a48e0d40961cd                                                0.0s
 => => naming to docker.io/library/omni                                                                                                     0.0s
```

* 目前未尝试阻塞


## TODO
- [ ] 麦轮小车上要实现的
  - [ ] 多线程锁监测遥控离线
  - [ ] CMake,gitignore的的高级运用
  - [ ] `docker`镜像下运行代码,使用`start.sh`
- [x] 剩余代码任务
  - [x] UART遥控解算
  - [x] 写地盘
  - [x] 将线程包装成函数
  - [x] 遥控数据处理理解
  - [x] 云台
  - [x] 底盘跟随
- [x] 配置文件测试(net)
  - [x] 熟悉ini配置文件用法
  - [x] 读取修改代码
  - [x] 配合UDP+上位机修改配置文件
- [x] 遗留的坑
  - [x] UDP发送上位机
- [ ] ~~代码整理~~
  - [x] ~~socket CAN 通信整理~~
  - [ ] ~~RUI的代码吃透~~

## 文件

```
├── CMakeLists.txt
├── config.ini
├── Dockerfile
├── inc
│   ├── main.h
│   ├── Test.h
│   ├── YU_ATTACK.h
│   ├── YU_CAN.h
│   ├── YU_CHASSIS.h
│   ├── YU_CONFIG.h
│   ├── YU_DEBUG.h
│   ├── YU_DEFINE.h
│   ├── YU_GIMBAL.h
│   ├── YU_MATH.h
│   ├── YU_MOTOR.h
│   ├── YU_PID.h
│   ├── YU_ROOT_INIT.h
│   ├── YU_THREAD.h
│   ├── YU_TOP.h
│   ├── YU_UART.h
│   └── YU_VOFA.h
├── README.md
├── src
│   ├── main.cpp
│   ├── Test.cpp
│   ├── YU_ATTACK.cpp
│   ├── YU_CAN.cpp
│   ├── YU_CHASSIS.cpp
│   ├── YU_CONFIG.cpp
│   ├── YU_DEBUG.cpp
│   ├── YU_GIMBAL.cpp
│   ├── YU_MATH.cpp
│   ├── YU_MOTOR.cpp
│   ├── YU_PID.cpp
│   ├── YU_ROOT_INIT.cpp
│   ├── YU_THREAD.cpp
│   ├── YU_TOP.cpp
│   ├── YU_UART.cpp
│   └── YU_VOFA.cpp
└── start.sh

15 directories, 86 files

```

## 已解决的问题

- [x] 线程3跑的最快，出现下面运行结果；

*原因：* 这是`CAN INIT`的运行结果，因为没插`CAN`口，在

```c++
if(ioctl(YU_C_FD[0],SIOCGIFINDEX,&YU_C_IFR[0]) < 0)
    {
        perror("Please check if CAN1 EXISTS \n");
        close(YU_C_FD[0]);
        exit(-1);
    }
```

直接退出代码。

*解决方案：* 先注释掉这个。

*经测试，编译后成功进入1，2，4线程。2线程没有关闭套接字故将一直循环。*

- [x] 遇到了奇怪的问题，学校里写的UDP发VOFA好像不能用了。

先是`sendto error`，后面直接`bind error`。改了`ip`还是没用。

```c++
SERVER_ADDR.sin_addr.s_addr = inet_addr("119.178.43.143");
```

*原因：* `Vscode`编译器强大，原来写的代码部分有误，但是被编译成功了，就没注意到。

```c
int SERVER_FD = 0;
struct sockaddr_in SERVER_ADDR = {}, CLIENT_ADDR = {};
socklen_t CLIENT_ADDR_LEN;
```
应改成
```c
int SERVER_FD = 0;
struct sockaddr_in SERVER_ADDR = {}, CLIENT_ADDR = {};
socklen_t CLIENT_ADDR_LEN = sizeof (CLIENT_ADDR);
```

~~- [ ] `main.cpp`中线程3注释掉的部分编译不过~~
- [x] UDP 解算一点也没看懂，就没写

这个需要整理的有点多，整理先放放，先把地盘搓了。

- [x] 只有这一个线程跑了，目前没有看到其他线程运行

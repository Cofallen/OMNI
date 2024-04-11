# RUI的作业

## 日志

* 2024-01-20 今天基本完成RUI留的作业，距离回家还有两个小时，抓紧准备一下
* 2024-01-21 现在主要还是线程安排，socket CAN 的应用。不知道为什么测试线程没run。
* 2024-02-25 解决了VOFA不能用的问题
* 2024-03-02 `CONFIG`编译成功
* 2024-03-08 `PID`基本完成，初始化读取`CONFIG`还是有点迷糊
* 2024-04-10 can初始化需要分阶段插，找不同`id`，另外，不要用反向`2pin`线

## 问题

- [x] 线程3跑的最快，出现下面运行结果；

```bash
/tmp/tmp.3rxswWp9QV/build/debug/demo
开始CAN初始化......
CAN FD build OK
Please check if CAN1 EXISTS 
: No such device
```

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


## 基本整理
[1.socket CAN 通信的基本配置]()

[2.多线程的高级运用]()

[3.CMake,gitignore的的高级运用]()

[4.RUI的代码分析]()


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
├── build
│   └── debug
│       ├── CMakeCache.txt
│       ├── CMakeFiles
│       │   ├── 3.18.4
│       │   │   ├── CMakeCCompiler.cmake
│       │   │   ├── CMakeCXXCompiler.cmake
│       │   │   ├── CMakeDetermineCompilerABI_C.bin
│       │   │   ├── CMakeDetermineCompilerABI_CXX.bin
│       │   │   ├── CMakeSystem.cmake
│       │   │   ├── CompilerIdC
│       │   │   │   ├── a.out
│       │   │   │   ├── CMakeCCompilerId.c
│       │   │   │   └── tmp
│       │   │   └── CompilerIdCXX
│       │   │       ├── a.out
│       │   │       ├── CMakeCXXCompilerId.cpp
│       │   │       └── tmp
│       │   ├── clion-environment.txt
│       │   ├── cmake.check_cache
│       │   ├── CMakeDirectoryInformation.cmake
│       │   ├── CMakeOutput.log
│       │   ├── CMakeTmp
│       │   ├── demo.dir
│       │   │   ├── build.make
│       │   │   ├── cmake_clean.cmake
│       │   │   ├── CXX.includecache
│       │   │   ├── DependInfo.cmake
│       │   │   ├── depend.internal
│       │   │   ├── depend.make
│       │   │   ├── flags.make
│       │   │   ├── link.txt
│       │   │   ├── progress.make
│       │   │   └── src
│       │   │       ├── main.cpp.o
│       │   │       ├── Test.cpp.o
│       │   │       ├── YU_ATTACK.cpp.o
│       │   │       ├── YU_CAN.cpp.o
│       │   │       ├── YU_CHASSIS.cpp.o
│       │   │       ├── YU_CONFIG.cpp.o
│       │   │       ├── YU_DEBUG.cpp.o
│       │   │       ├── YU_GIMBAL.cpp.o
│       │   │       ├── YU_MATH.cpp.o
│       │   │       ├── YU_MOTOR.cpp.o
│       │   │       ├── YU_PID.cpp.o
│       │   │       ├── YU_ROOT_INIT.cpp.o
│       │   │       ├── YU_THREAD.cpp.o
│       │   │       ├── YU_TOP.cpp.o
│       │   │       ├── YU_UART.cpp.o
│       │   │       └── YU_VOFA.cpp.o
│       │   ├── Makefile2
│       │   ├── Makefile.cmake
│       │   ├── progress.marks
│       │   └── TargetDirectories.txt
│       ├── cmake_install.cmake
│       ├── demo
│       ├── demo.cbp
│       ├── Makefile
│       └── Testing
│           └── Temporary
│               └── LastTest.log
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
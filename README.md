# RUI的作业

## 日志

* 2024-01-20 今天基本完成RUI留的作业，距离回家还有两个小时，抓紧准备一下
* 2024-01-21 现在主要还是线程安排，socket CAN 的应用。不知道为什么测试线程没run。
* 2024-02-25 解决了VOFA不能用的问题
* 2024-03-02 `CONFIG`编译成功
* 2024-03-08 `PID`基本完成，初始化读取`CONFIG`还是有点迷糊


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

- [ ] 剩余代码任务
  - [x] UART遥控解算
  - [x] 写地盘
  - [x] 将线程包装成函数
  - [ ] 遥控数据处理理解
  - [ ] 云台
  - [ ] 底盘跟随
- [ ] 配置文件测试(net)
  - [x] 熟悉ini配置文件用法
  - [x] 读取修改代码
  - [ ] 配合UDP+上位机修改配置文件
- [ ] 遗留的坑
  - [ ] UDP发送上位机
- [ ] 代码整理
  - [ ] socket CAN 通信整理
  - [ ] 多线程的高级运用
  - [ ] CMake,gitignore的的高级运用
  - [ ] RUI的代码吃透


不要看，可能记错了
```shell
ls /dev
ls -l /dev | grep ttyTH
* sudo chmod 777 /dev/ttyTHS0 
ls -l /dev | grep ttyTH
cat  /dev/ttyTHS0
```

```shell
cd /omni/build/debug # 调试的路径

cd /omni
ls grep tty          # 具体的忘了，

ls omni | grep tty   # 大写小写忘了

TTYS0                # 串口
# ls -l ttys0
chomd 777 ttys0

```
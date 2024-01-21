# RUI的作业

## 日志

* 2024-01-20 今天基本完成RUI留的作业，距离回家还有两个小时，抓紧准备一下
* 2024-01-24 现在主要还是线程安排，socket CAN 的应用。不知道为什么测试线程没run。
* 


## 问题

* 线程3跑的最快，出现下面运行结果；

```bash
/tmp/tmp.3rxswWp9QV/build/debug/demo
开始CAN初始化......
CAN FD build OK
Please check if CAN1 EXISTS 
: No such device
```

* `main.cpp`中线程3注释掉的部分编译不过
* UDP 解算一点也没看懂，就没写
* 只有这一个线程跑了，目前没有看到其他线程运行


## 基本整理
[1.socket CAN 通信的基本配置]()

[2.多线程的高级运用]()

[3.CMake,gitignore的的高级运用]()

[4.RUI的代码分析]()


## TODO

- [ ] RUI留的作业
    - [ ] socket CAN 通信整理
    - [ ] 多线程的高级运用
    - [ ] CMake,gitignore的的高级运用
    - [ ] RUI的代码吃透


- [ ] 我的小计划
  - [ ] 目前没想好
  - [ ] 弄清终端只有THREAD_3运行的原因。完成测试线程的运行。
  - [ ] 弄清THREAD_3的终端反馈。
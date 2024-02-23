#pragma once

#include <spdlog/spdlog.h>

#include <csignal>

/******************************************************
 * 一、信号是一种软中断，信号机制是进程间通信的一种方式，采用异步通信方式
 *
 * 二、信号的种类
 *    对于signal信号，绝大部分的默认处理都是终止进程或停止进程，或
 *    dump内核映像转储。其中 1-31的信号为非实时信号，32-64位实时信号。
 *
 *    信号分为可靠信号与不可靠信号,可靠信号又称为实时信号，非可靠信号又称为非实时信号。
 *    号代码从1到32是不可靠信号,不可靠信号主要有以下问题:
 *    (1)每次信号处理完之后，就会恢复成默认处理，这可能是调用者不希望看到的
 *    (2)存在信号丢失的问题
 *    现在的Linux对信号机制进行了改进，因此，不可靠信号主要是指信号丢失。
 *    信号代码从SIGRTMIN到SIGRTMAX之间的信号是可靠信号。可靠信号不存在丢失，由sigqueue发送，可靠信号支持排队。
 *
 *  1	 SIGHUP	    挂起
 *  2	 SIGINT	    中断
 *  3	 SIGQUIT	退出
 *  4	 SIGILL	    非法指令
 *  5	 SIGTRAP	断点或陷阱指令
 *  6	 SIGABRT	abort发出的信号
 *  7	 SIGBUS	    非法内存访问
 *  8	 SIGFPE	    浮点异常
 *  9	 SIGKILL	kill信号	不能被忽略、处理和阻塞
 *  10	 SIGUSR1	用户信号1
 *  11	 SIGSEGV	无效内存访问
 *  12	 SIGUSR2	用户信号2
 *  13	 SIGPIPE	管道破损，没有读端的管道写数据
 *  14	 SIGALRM	alarm发出的信号
 *  15	 SIGTERM	终止信号
 *  16	 SIGSTKFLT	栈溢出
 *  17	 SIGCHLD	子进程退出	默认忽略
 *  18	 SIGCONT	进程继续
 *  19	 SIGSTOP	进程停止	不能被忽略、处理和阻塞
 *  20	 SIGTSTP	进程停止
 *  21	 SIGTTIN	进程停止，后台进程从终端读数据时
 *  22	 SIGTTOU	进程停止，后台进程想终端写数据时
 *  23	 SIGURG	    I/O有紧急数据到达当前进程	默认忽略
 *  24	 SIGXCPU	进程的CPU时间片到期
 *  25	 SIGXFSZ	文件大小的超出上限
 *  26	 SIGVTALRM	虚拟时钟超时
 *  27	 SIGPROF	profile时钟超时
 *  28	 SIGWINCH	窗口大小改变	默认忽略
 *  29	 SIGIO	    I/O相关
 *  30	 SIGPWR	    关机	默认忽略
 *  31	 SIGSYS	    系统调用异常
 *
 * 三、信号的产生
 *    大致分为硬件类和软件类两种
 *    1、硬件类：大致包括在终端输入组合键ctrl+C产生SIGINT信号，
 *              CPU检测到内存非法访问等异常，通知内核生成相应信号。
 *    2、软件类：通过系统调用，发送signal信号：kill(),raise(),
 *              sigqueue(),alarm(),setitimer(),abort()
 *
 * 四、信号的注册
 *
 ******************************************************/

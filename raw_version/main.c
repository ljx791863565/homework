#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <cassert>
#include <sys/epoll.h>

#include "./lock/locker.h"
#include "./threadpool/threadpool.h"
#include "./timer/lst_timer.h"
#include "./http/http_conn.h"
#include "./log/log.h"
#include "./CGImysql/sql_connection_pool.h"

#define MAX_FD 65536           //最大文件描述符
#define MAX_EVENT_NUMBER 10000 //最大事件数
#define TIMESLOT 5             //最小超时单位

#define SYNSQL //同步数据库校验
//#define CGISQLPOOL    //CGI数据库校验
#define SYNLOG //同步写日志
//#define ASYNLOG       //异步写日志

//#define ET            //边缘触发非阻塞
#define LT //水平触发阻塞

//这三个函数在http_conn.cpp中定义，改变链接属性
extern int addfd(int epollfd, int fd, bool one_shot);
extern int remove(int epollfd, int fd);
extern int setnonblocking(int fd);

//设置定时器相关参数
static int pipefd[2];	//管道 pipefd[0]读管道; pipefd[1]写管道
static sort_timer_lst timer_lst;
static int epollfd = 0;

//信号处理函数
void sig_handler(int sig)
{
    //为保证函数的可重入性，保留原来的errno
    int save_errno = errno;
    int msg = sig;
	/*
	 * int send(SOCKET s, const char FAR *buf, int len, int flags);
	 * 一般由客户端向服务器发出请求 这里指将msg信息写入到管道
	 * 
	 * 参数
	 * 该函数的第一个参数指定发送端套接字描述符；
	 * 第二个参数指明一个存放应用程序要发送数据的缓冲区；
	 * 第三个参数指明实际要发送的数据的字节数；
	 * 第四个参数一般置0
	 *
	 * 执行时
	 * i)send先比较待发送数据的长度len和套接字s的发送缓冲的长度，如果len大于s的发送缓冲区的长度，该函数返回SOCKET_ERROR；
	 * ii)如果len小于或者等于s的发送缓冲区的长度，那么send先检查协议是否正在发送s的发送缓冲中的数据 如果是就等待协议把数据发送完
	 * 如果协议还没有开始发送s的发送缓冲中的数据或者s的发送缓冲中没有数据 那么send就比较s的发送缓冲区的剩余空间和len
	 * iii)如果len大于剩余空间大小，send就一直等待协议把s的发送缓冲中的数据发送完
	 * iv)如果len小于剩余 空间大小，send就仅仅把buf中的数据copy到剩余空间里（注意并不是send把s的发送缓冲中的数据传到连接的另一端的
	 * 而是协议传的，send仅仅是把buf中的数据copy到s的发送缓冲区的剩余空间里）。
	 *
	 * 返回值
	 * 如果send函数copy数据成功，就返回实际copy的字节数
	 * 如果send在copy数据时出现错误，那么send就返回SOCKET_ERROR；
	 * 如果send在等待协议传送数据时网络断开的话，那么send函数也返回SOCKET_ERROR。
	 *
	 * 要注意send函数把buf中的数据成功copy到s的发送缓冲的剩余空间里后它就返回了，但是此时这些数据并不一定马上被传到连接的另一端
	 * 在Unix系统下，如果send在等待协议传送数据时网络断开的话，调用send的进程会接收到一个SIGPIPE信号，进程对该信号的默认处理是进程终止
	 */
    send(pipefd[1], (char *)&msg, 1, 0);
    errno = save_errno;
}

//设置信号函数
void addsig(int sig, void(handler)(int), bool restart = true)
{
	/*
	 * 信号安装函数 sigaction(int signum,const struct sigaction *act,struct sigaction *oldact);
	 * 后两个参数可为空
	 *
	 * 对于内核头文件 struct sigaction 结构体定义在kernel/include/asm/signal.h,此头文件又被kernel/include/linux/signal.h包含
	 * 对于用户空间头文件 struct sigaction定义在 /usr/include/bits/sigaction.h,此头文件又被/usr/include/signal.h包含
	 * 所以应用程序中如果用到此结构，只要#include <signal.h>即可、
	 * 内核空间的sigaction结构只支持函数类型为__sighandler_t的信号处理函数，不能处理信号传递的额外信息。
	 *
	 * // Type of a signal handler. 
　　typedef void (*__sighandler_t)(int);
　　#ifdef __KERNEL__
　　struct old_sigaction {
          __sighandler_t sa_handler;
         old_sigset_t sa_mask;
         unsigned long sa_flags;
         void (*sa_restorer)(void);
　　};
　　struct sigaction {
         __sighandler_t sa_handler;
        unsigned long sa_flags;
        void (*sa_restorer)(void);
        sigset_t sa_mask;   // mask last for extensibility 
　　};
　　struct k_sigaction {
        struct sigaction sa;
　　};
　　#else
　　// Here we must cater to libcs that poke about in kernel headers.
　　struct sigaction {
          union {
                __sighandler_t _sa_handler;
                void (*_sa_sigaction)(int, struct siginfo *, void *);
          } _u;
          sigset_t sa_mask;
          unsigned long sa_flags;
          void (*sa_restorer)(void);
　　};
　　#define sa_handler   _u._sa_handler
　　#define sa_sigaction _u._sa_sigaction
　　#endif // __KERNEL__ 
	 *
	 * sa_handler的原型是一个参数为int，返回类型为void的函数指针。参数即为信号值，所以信号不能传递除信号值之外的任何信息;
	 * sa_sigaction的原型是一个带三个参数，类型分别为int，struct siginfo *，void *返回类型为void的函数指针。
	 * 第一个参数为信号值;第二个参数是一个指向struct siginfo结构的指针，此结构中包含信号携带的数据值;第三个参数没有使用
	 * sa_mask 指定在信号处理程序执行过程中，哪些信号应当被阻塞。默认当前信号本身被阻塞。
	 * sa_flags包含了许多标志位，比较重要的一个是SA_SIGINFO 当设定了该标志位时，表示信号附带的参数可以传递到信号处理函数中。
	 * 即使sa_sigaction指定信号处理函数，如果不设置SA_SIGINFO，信号处理函数同样不能得到信号传递过来的数据
	 * 在信号处理函数中对这些信息的访问都将导致段错误。
	 * sa_restorer已过时，POSIX不支持它，不应再使用。
	 *
	 * 当你的信号需要接收附加信息的时候，你必须给sa_sigaction赋信号处理函数指针，同时还要给sa_flags赋SA_SIGINFO
	 #include <signal.h>
     ……
     void sig_handler_with_arg(int sig,siginfo_t *sig_info,void *unused){……}
    
     int main(int argc,char **argv)
     {
			  struct sigaction sig_act;
              // …… do something
              sigemptyset(&sig_act.sa_mask);	//将参数sig_act的信号集初始化并清空 执行成功则返回0，如果有错误则返回-1
              sig_act.sa_sigaction=sig_handler_with_arg;
              sig_act.sa_flags=SA_SIGINFO;
              // …… do something
     }
	 *
	 * 当你的应用程序只需要接收信号，而不需要接收额外信息，那你需要的设置的是sa_handler,而不是sa_sigaction
	 * #include <signal.h>
     ……
     void sig_handler(int sig){……}
    
     int main(int argc,char **argv)
     {
              struct sigaction sig_act;
              ……
              sigemptyset(&sig_act.sa_mask);
              sig_act.sa_handler=sig_handler;
              sig_act.sa_flags=0; 
               ……
      }
	 */
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = handler;
	/*
	 * 一般情况下进程正在执行某个系统调用 那么在该系统调用返回前信号是不会被递送的。
	 * 但慢速系统调用除外 如读写终端、网络、磁盘，以及wait和pause。这些系 统调用都会返回-1，errno置为EINTR当系统调用被中断时
	 * 我们可以选择使用循环再次调用 或者设置重新启动该系统调用 (SA_RESTART)。
	 * 旦给信号设置了SA_RESTART标记，那么当执行某个阻塞系统调用时，收到该信号时，进程不会返回，而是重新执行该系统调用
	 */
    if (restart)
        sa.sa_flags |= SA_RESTART;
	/*
	 * 宏实现 将信号集初始化然后把所有的信号加入到此信号集里即将所有的信号标志位置为1 屏蔽所有的信号
	 * #define sigfillset(ptr) ( *(ptr) = ~(sigset_t)0, 0)
	 * 逗号运算符后面的值(即0)作为表达式的值返回 所以此函数必须返回0
	 */
    sigfillset(&sa.sa_mask);
    assert(sigaction(sig, &sa, NULL) != -1);
}

//定时处理任务，重新定时以不断触发SIGALRM信号
void timer_handler()
{
    timer_lst.tick();
    alarm(TIMESLOT);
}

//定时器回调函数，删除非活动连接在socket上的注册事件，并关闭
void cb_func(client_data *user_data)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, user_data->sockfd, 0);
    assert(user_data);
    close(user_data->sockfd);
    http_conn::m_user_count--;
    LOG_INFO("close fd %d", user_data->sockfd);
    Log::get_instance()->flush();
}

void show_error(int connfd, const char *info)
{
    printf("%s", info);
    send(connfd, info, strlen(info), 0);
    close(connfd);
}

int main(int argc, char *argv[])
{

#ifdef ASYNLOG
    Log::get_instance()->init("ServerLog", 2000, 800000, 8); //异步日志模型
#endif

#ifdef SYNLOG
    Log::get_instance()->init("ServerLog", 2000, 800000, 0); //同步日志模型
#endif

    if (argc <= 1)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return 1;
    }

    int port = atoi(argv[1]);

    addsig(SIGPIPE, SIG_IGN);

    //创建数据库连接池
    connection_pool *connPool = connection_pool::GetInstance();
    connPool->init("localhost", "root", "0321", "ljxDB", 3306, 8);

    //创建线程池
    threadpool<http_conn> *pool = NULL;
    try
    {
        pool = new threadpool<http_conn>(connPool);
    }
    catch (...)
    {
        return 1;
    }

    http_conn *users = new http_conn[MAX_FD];
    assert(users);

#ifdef SYNSQL
    //初始化数据库读取表
    users->initmysql_result(connPool);
#endif

#ifdef CGISQLPOOL
    //初始化数据库读取表
    users->initresultFile(connPool);
#endif

 
    int listenfd = socket(PF_INET, SOCK_STREAM, 0);
    assert(listenfd >= 0);

    //struct linger tmp={1,0};
    //SO_LINGER若有数据待发送，延迟关闭
    //setsockopt(listenfd,SOL_SOCKET,SO_LINGER,&tmp,sizeof(tmp));

    int ret = 0;
    struct sockaddr_in address;
    bzero(&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);


    int flag = 1;
    setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag));
    ret = bind(listenfd, (struct sockaddr *)&address, sizeof(address));
    assert(ret >= 0);
    ret = listen(listenfd, 5);
    assert(ret >= 0);

    //创建内核事件表
    epoll_event events[MAX_EVENT_NUMBER];
    epollfd = epoll_create(5);
    assert(epollfd != -1);

    addfd(epollfd, listenfd, false);
    http_conn::m_epollfd = epollfd;

    //创建管道
    ret = socketpair(PF_UNIX, SOCK_STREAM, 0, pipefd);
    assert(ret != -1);
    setnonblocking(pipefd[1]);
    addfd(epollfd, pipefd[0], false);

    addsig(SIGALRM, sig_handler, false);
    addsig(SIGTERM, sig_handler, false);
    bool stop_server = false;

    client_data *users_timer = new client_data[MAX_FD];

    bool timeout = false;
    alarm(TIMESLOT);

    while (!stop_server)
    {
        int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if (number < 0 && errno != EINTR)
        {
            LOG_ERROR("%s", "epoll failure");
            break;
        }

        for (int i = 0; i < number; i++)
        {
            int sockfd = events[i].data.fd;

            //处理新到的客户连接
            if (sockfd == listenfd)
            {
                struct sockaddr_in client_address;
                socklen_t client_addrlength = sizeof(client_address);
#ifdef LT
                int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
                if (connfd < 0)
                {
                    LOG_ERROR("%s:errno is:%d", "accept error", errno);
                    continue;
                }
                if (http_conn::m_user_count >= MAX_FD)
                {
                    show_error(connfd, "Internal server busy");
                    LOG_ERROR("%s", "Internal server busy");
                    continue;
                }
                users[connfd].init(connfd, client_address);

                //初始化client_data数据
                //创建定时器，设置回调函数和超时时间，绑定用户数据，将定时器添加到链表中
                users_timer[connfd].address = client_address;
                users_timer[connfd].sockfd = connfd;
                util_timer *timer = new util_timer;
                timer->user_data = &users_timer[connfd];
                timer->cb_func = cb_func;
                time_t cur = time(NULL);
                timer->expire = cur + 3 * TIMESLOT;
                users_timer[connfd].timer = timer;
                timer_lst.add_timer(timer);
#endif

#ifdef ET
                while (1)
                {
                    int connfd = accept(listenfd, (struct sockaddr *)&client_address, &client_addrlength);
                    if (connfd < 0)
                    {
                        LOG_ERROR("%s:errno is:%d", "accept error", errno);
                        break;
                    }
                    if (http_conn::m_user_count >= MAX_FD)
                    {
                        show_error(connfd, "Internal server busy");
                        LOG_ERROR("%s", "Internal server busy");
                        break;
                    }
                    users[connfd].init(connfd, client_address);

                    //初始化client_data数据
                    //创建定时器，设置回调函数和超时时间，绑定用户数据，将定时器添加到链表中
                    users_timer[connfd].address = client_address;
                    users_timer[connfd].sockfd = connfd;
                    util_timer *timer = new util_timer;
                    timer->user_data = &users_timer[connfd];
                    timer->cb_func = cb_func;
                    time_t cur = time(NULL);
                    timer->expire = cur + 3 * TIMESLOT;
                    users_timer[connfd].timer = timer;
                    timer_lst.add_timer(timer);
                }
                continue;
#endif
            }

            else if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR))
            {
                //服务器端关闭连接，移除对应的定时器
                util_timer *timer = users_timer[sockfd].timer;
                timer->cb_func(&users_timer[sockfd]);
                
                if (timer)
                {
                    timer_lst.del_timer(timer);
                }
            }

            //处理信号
            else if ((sockfd == pipefd[0]) && (events[i].events & EPOLLIN))
            {
                int sig;
                char signals[1024];
                ret = recv(pipefd[0], signals, sizeof(signals), 0);
                if (ret == -1)
                {
                    continue;
                }
                else if (ret == 0)
                {
                    continue;
                }
                else
                {
                    for (int i = 0; i < ret; ++i)
                    {
                        switch (signals[i])
                        {
                        case SIGALRM:
                        {
                            timeout = true;
                            break;
                        }
                        case SIGTERM:
                        {
                            stop_server = true;
                        }
                        }
                    }
                }
            }

            //处理客户连接上接收到的数据
            else if (events[i].events & EPOLLIN)
            {
                util_timer *timer = users_timer[sockfd].timer;
                if (users[sockfd].read_once())
                {
                    LOG_INFO("deal with the client(%s)", inet_ntoa(users[sockfd].get_address()->sin_addr));
                    Log::get_instance()->flush();
                    //若监测到读事件，将该事件放入请求队列
                    pool->append(users + sockfd);

                    //若有数据传输，则将定时器往后延迟3个单位
                    //并对新的定时器在链表上的位置进行调整
                    if (timer)
                    {
                        time_t cur = time(NULL);
                        timer->expire = cur + 3 * TIMESLOT;
                        LOG_INFO("%s", "adjust timer once");
                        Log::get_instance()->flush();
                        timer_lst.adjust_timer(timer);
                    }
                }
                else
                {
                    timer->cb_func(&users_timer[sockfd]);
                    if (timer)
                    {
                        timer_lst.del_timer(timer);
                    }
                }
            }
            else if (events[i].events & EPOLLOUT)
            {
                util_timer *timer = users_timer[sockfd].timer;
                if (users[sockfd].write())
                {
                    LOG_INFO("send data to the client(%s)", inet_ntoa(users[sockfd].get_address()->sin_addr));
                    Log::get_instance()->flush();

                    //若有数据传输，则将定时器往后延迟3个单位
                    //并对新的定时器在链表上的位置进行调整
                    if (timer)
                    {
                        time_t cur = time(NULL);
                        timer->expire = cur + 3 * TIMESLOT;
                        LOG_INFO("%s", "adjust timer once");
                        Log::get_instance()->flush();
                        timer_lst.adjust_timer(timer);
                    }
                }
                else
                {
                    timer->cb_func(&users_timer[sockfd]);
                    if (timer)
                    {
                        timer_lst.del_timer(timer);
                    }
                }
            }
        }
        if (timeout)
        {
            timer_handler();
            timeout = false;
        }
    }
    close(epollfd);
    close(listenfd);
    close(pipefd[1]);
    close(pipefd[0]);
    delete[] users;
    delete[] users_timer;
    delete pool;
    return 0;
}

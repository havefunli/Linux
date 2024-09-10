#include <iostream>
#include <unistd.h>
#include <signal.h>


// 自定义函数
void signal_handler(int signum)
{
    std::cout << "Recived signal SIGINT!!!" << std::endl;
}

int main()
{
    // 捕获信号
    signal(SIGINT, signal_handler);
    
    sigset_t sigset;
    // 初始化信号集
    sigemptyset(&sigset);
    // 添加指定信号到信号集
    sigaddset(&sigset, SIGINT);

    // 阻塞该信号
    if(sigprocmask(SIG_BLOCK, &sigset, NULL) == -1)
    {
        perror("sigprocmask");
    }

    std::cout << "SIGINT is blocked. Try pressing Ctrl+C after 5s!!!\n" << std::endl;
    sleep(5);
    
    // 解除阻塞
    if(sigprocmask(SIG_UNBLOCK, &sigset, NULL) == -1)
    {
        perror("sigprocmask");
    }

    std::cout << "SIGINT is unblocked. Try pressing Ctrl+C!!!\n" << std::endl;
 
    while(true)
    {
        sleep(1);
    }

    return 0;
}

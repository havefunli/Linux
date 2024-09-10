#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void signal_handle(int signum)
{
    std::cout << "Your alarm clock is ringing." << std::endl;
    alarm(2);
}

int main()
{

    // 设置一个闹钟，执行默认操作
    alarm(2);
    // 捕获闹钟信号
    signal(SIGALRM, signal_handle);

    while(true)
    {
        std::cout << "I am Running, my pid is " << getpid() << std::endl;
        sleep(1);
    }
    return 0;
}

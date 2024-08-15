#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

void signal_handle(int signum)
{
    std::cout << "I got you signal: " << signum << std::endl;
}

int main()
{

    // 2号信号的捕获
    signal(2, signal_handle);

    while(true)
    {
        std::cout << "I am Running, my pid is " << getpid() << std::endl;
        sleep(1);
    }
    return 0;
}

#include <sys/fcntl.h>
#include <iostream>
#include <cstdio>

bool SetNonBlock(int fd)
{
    int fl = fcntl(fd, F_GETFL);  // 获取当前标志
    if (fl < 0)
    {
        perror("fcntl");
        return false;
    }
    
    // 设置 O_NONBLOCK 标志
    if (fcntl(fd, F_SETFL, fl | O_NONBLOCK) < 0)
    {
        perror("fcntl");
        return false;
    }

    std::cout << "Successfully set fd = " << fd << " to non-blocking..." << std::endl;
    return true;
}
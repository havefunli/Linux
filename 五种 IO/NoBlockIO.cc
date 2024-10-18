#include <iostream>
#include <unistd.h>
#include <fcntl.h>

bool SetNoBlock(int fd)
{
    if (fd < 0)
    {
        std::cerr << "error fd..." << std::endl;
        return false;
    }

    int fd_flags = fcntl(fd, F_GETFL, 0);
    if (fd_flags < 0)
    {
        perror("fcntl:");
        return false;
    }


    if (fcntl(fd, F_SETFL, fd_flags | O_NONBLOCK))
    {
        perror("fcntl:");
        return false;
    }

    return true;
}

int main()
{
    char buf[1024];
    if (!SetNoBlock(0))
    {
        perror("set error:");
    }

    while (true)
    {
        int n = read(0, buf, sizeof(buf) - 1);
        if (n > 0)
        {
            buf[n] = '\0';
            std::cout << buf << std::endl;
        }
        else
        {
            if (errno == EWOULDBLOCK)
            {
                // do other
                std::cout << "暂时还没有数据处理！" << std::endl;
                sleep(1);
            }
            else
            {
                perror("read:");
                break;
            }
        }
    }

    return 0;
}

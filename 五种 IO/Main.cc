#include "SelectServer.hpp"


// int main(int argc, char *argv[])
// {
//     if (argc != 2)
//     {
//         std::cerr << "Usage: ./server port" << std::endl;
//         return -1;
//     }

//     uint16_t port = std::stoi(argv[1]);
//     std::unique_ptr<SelectServer> svrptr = std::make_unique<SelectServer>(port);
//     svrptr->Loop();
    
//     return 0;
// }

int main()
{
    int sockfd = 0; // 假设我们现在获取了一个套接字文件

    fd_set readfds;
    // 初始化集合
    FD_ZERO(&readfds);
    // 加入我们需要监视的 fd
    FD_SET(sockfd, &readfds);

    // 设置超时时间为 2s
    struct timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = 0;


    while(true)
    {
        int n = select(sockfd + 1, &readfds, 0, 0, &timeout);

        if (n == -1)
        {
            perror("select:");
            break;
        }
        else if (n == 0)
        {
            std::cout << "时间超时了，但也没有数据！" <<std::endl;
            // do other;
        }
        else
        {
            std::cout << "你的数据准备好了, 快来取哦！" <<std::endl;
        }
    }

    return 0;
}
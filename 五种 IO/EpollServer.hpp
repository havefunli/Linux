#include "ToolForSockAddr_in.hpp"
#include "Socket.hpp"
#include <sys/epoll.h>

const int MaxSize = 1024;

class EpollServer
{
private:
    struct epoll_event InitEpollEvent(int fd, uint32_t event)
    {
        struct epoll_event fd_event;
        fd_event.data.fd = fd;
        fd_event.events = event;
        
        return fd_event;
    }


public:
    EpollServer(uint16_t port)
        : _port(port)
        , _listensock(std::make_unique<TcpSocket>())
    {
        _listensock->BuildTcpListen(_port);
        std::cout << "Successful init EpollServer..." << std::endl;

        // 创建 epoll
        _epollfd = epoll_create(1);
        if (_epollfd < 0)
        {
            perror("epoll_create:");
            exit(1);
        }
        std::cout << "Successful epoll create..." << std::endl;
        
        // 监控监听套接字文件
        int fd = _listensock->GetFd();
        struct epoll_event fd_event = InitEpollEvent(fd, EPOLLIN);
        epoll_ctl(_epollfd, EPOLL_CTL_ADD, fd, &fd_event);
    }

    void Accepter()
    {
        // 获取最新连接的套接字
        SockAddHelper Client;
        int newsockfd = _listensock->AcceptConnect(Client)->GetFd();
        struct epoll_event event= InitEpollEvent(newsockfd, EPOLLIN);

        // 加入 epoll 管理
        if (epoll_ctl(_epollfd, EPOLL_CTL_ADD, newsockfd, &event) < 0)
        {
            perror("epoll_ctl:");
            return;
        }

        std::cout << "Successful add fd = " << newsockfd << ", the client is " << Client.GetIP() << std::endl;
    }

    void HandleIO(int index)
    {
        char Buf[MaxSize];
        int fd = _events[index].data.fd;
        int n = recv(fd, Buf, MaxSize, 0);
        
        // 客户端退出逻辑
        if (n == 0)
        {
            std::cout << "Client quit..." << std::endl;
            close(fd);
        }
        else if(n > 0)
        {
            Buf[n] = '\0';
            std::cout << Buf << std::endl;
        }
        else
        {
            perror("recv:");
        }
    }

    // 文件描述符处理逻辑
    void HandleEvent(int n)
    {
        for (int i = 0; i < n; i++)
        {
            if (_events[i].data.fd == _listensock->GetFd())
            {
                Accepter();
            }
            else
            {
                HandleIO(i);
            }
        }

        std::cout << "Successful handle all fd..." << std::endl;
    }

    void Loop()
    {
        int timeout = 1000;
        while (true)
        {
            int n = epoll_wait(_epollfd, _events, MaxSize, 3000);
            switch(n)
            {
                case 0:
                    std::cout << "超时了..." << std::endl;
                    break;
                case -1:
                    perror("epoll_wait:");
                    break;
                default:
                    std::cout << n << " 个读事件已经就绪，尽快处理！" << std::endl;
                    HandleEvent(n);
                    break;
            }
        }
    }

private:
    uint16_t _port;
    int _epollfd;
    struct epoll_event _events[MaxSize];
    std::unique_ptr<Socket> _listensock;
};
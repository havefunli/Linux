#include "ToolForSockAddr_in.hpp"
#include "Socket.hpp"
#include <poll.h>
#include <vector>

const int MaxSize = 1024;

class PollServer
{
private:
    struct pollfd InitPollFd(int fd, short event)
    {
        struct pollfd fd_event;
        fd_event.fd = fd;
        fd_event.events = event;

        return fd_event;
    }

public:
    PollServer(uint16_t port)
        : _port(port)
        , _listensock(std::make_unique<TcpSocket>())
    {
        _listensock->BuildTcpListen(_port);
        std::cout << "Successful init PollServer..." << std::endl;

        // 监控监听套接字文件
        int listensockfd = _listensock->GetFd();
        struct pollfd fd_event = InitPollFd(listensockfd, POLLIN);
        _fds.push_back(fd_event);
        std::cout << "Successful add listenfd..." << std::endl;
    }

    void Accepter()
    {
        // 获取最新连接的套接字
        SockAddHelper Client;
        int newsockfd = _listensock->AcceptConnect(Client)->GetFd();
        // if (_fds.size() == MaxSize) 
        // {
        //     std::cout << "FD_SET is full..." << std::endl;
        //     return;
        // }

        _fds.push_back(InitPollFd(newsockfd, POLLIN));

        std::cout << "Successful add fd = " << newsockfd << ", the client is " << Client.GetIP() << std::endl;
    }

    void HandleIO(int index)
    {
        char Buf[MaxSize];
        int n = recv(_fds[index].fd, Buf, MaxSize, 0);
        
        // 客户端退出逻辑
        if (n == 0)
        {
            std::cout << "Client quit..." << std::endl;
            close(_fds[index].fd);
            _fds.erase(_fds.begin() + index);
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
    void HandleEvent()
    {
        for (int i = 0; i < _fds.size(); i++)
        {
            // 如果该文件已经就绪
            if (_fds[i].revents == POLLIN)
            {
                // 判断是否是 listenfd
                if (_fds[i].fd == _listensock->GetFd())
                {
                    Accepter();
                }
                else
                {
                    HandleIO(i);
                }
            }
        }

        std::cout << "Successful handle all fd..." << std::endl;
    }

    void Loop()
    {
        while (true)
        {
            int n = poll(_fds.data(), _fds.size(), -2);
            switch(n)
            {
                case 0:
                    std::cout << "超时了..." << std::endl;
                    break;
                case -1:
                    perror("select:");
                    break;
                default:
                    std::cout << n << " 个读事件已经就绪，尽快处理！" << std::endl;
                    HandleEvent();
                    break;
            }
        }
    }

private:
    uint16_t _port;
    std::vector<struct pollfd> _fds;
    std::unique_ptr<Socket> _listensock;
};
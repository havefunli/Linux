#include "ToolForSockAddr_in.hpp"
#include "Socket.hpp"
#include <vector>

const int MaxSize = 1024;

class SelectServer
{
private:
    void InitFdset(fd_set& fds)
    {
        int maxfd = -1;

        FD_ZERO(&fds);
        for (int i = 0; i < _fds.size(); i++)
        {
            FD_SET(_fds[i], &fds);
            if (_fds[i] > maxfd) maxfd = _fds[i];
        }
    }

    void PrintFdSet(fd_set& rfds)
    {
        for (auto& fd : _fds)
        {
            if (FD_ISSET(fd, &rfds))
            {
                std::cout << fd << " ";
            }
            std::cout << std::endl;
        }
    }

public:
    SelectServer(uint16_t port)
        : _port(port)
        , _listensock(std::make_unique<TcpSocket>())
    {
        _listensock->BuildTcpListen(_port);
        std::cout << "Successful init SelectServer..." << std::endl;

        // 监控监听套接字文件
        _fds.push_back(_listensock->GetFd());
        std::cout << "Successful add listenfd..." << std::endl;
        _maxfd = _listensock->GetFd() + 1;
    }

    void Accepter()
    {
        // 获取最新连接的套接字
        SockAddHelper Client;
        int newsockfd = _listensock->AcceptConnect(Client)->GetFd();
        if (_fds.size() == MaxSize) 
        {
            std::cout << "FD_SET is full..." << std::endl;
            return;
        }

        _fds.push_back(newsockfd);
        if (newsockfd > _maxfd) _maxfd = newsockfd;

        std::cout << "Successful add fd = " << newsockfd << ", the client is " << Client.GetIP() << std::endl;
    }

    void HandleIO(int index)
    {
        char Buf[MaxSize];
        int n = recv(_fds[index], Buf, MaxSize, 0);
        
        // 客户端退出逻辑
        if (n == 0)
        {
            std::cout << "Client quit..." << std::endl;
            close(_fds[index]);
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
    void HandleEvent(fd_set& rfds)
    {
        for (int i = 0; i < _fds.size(); i++)
        {
            // 如果该文件已经就绪
            if (FD_ISSET(_fds[i], &rfds))
            {
                // 判断是否是 listenfd
                if (_fds[i] == _listensock->GetFd())
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
            // 初始化 rfds，并将需要监控的文件描述符写入
            fd_set rfds;
            InitFdset(rfds);

            int n = select(_maxfd, &rfds, nullptr, nullptr, nullptr);
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
                    HandleEvent(rfds);
                    break;
            }
        }
    }

private:
    uint16_t _port;
    int _maxfd;
    std::vector<int> _fds;
    std::unique_ptr<Socket> _listensock;
};
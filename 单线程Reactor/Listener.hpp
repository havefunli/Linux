#include "Socket.hpp"
#include "Connection.hpp"
#include "ToolForSockAddr_in.hpp"
#include "Reactor.hpp"

class Listener
{
public:
    Listener(uint16_t Port)
        : _port(Port)
        , _listensockfd(std::make_unique<TcpSocket>())
    {
        _listensockfd->BuildTcpListen(_port);
    }

    void Accepter(Connection* conn)
    {
        // 底层是 ET 模式，所以需要一直读取
        // 直到数据被取走完
        while (true)
        {
            int status_code = 0;
            SockAddHelper client;
            int fd = _listensockfd->AcceptConnect(client, status_code);

            if (fd > 0)
            {
                conn->GetPtr()->AddConnection(fd, EPOLLIN, ConnType::NormalConnection, client);
            }
            else
            {
                // 读取完毕
                if (status_code == EWOULDBLOCK)
                {
                    std::cout << "Successful read all data..." << std::endl;
                    break;
                }
                // 中断
                else if (status_code == EINTR)
                {
                    continue;
                }
                // 异常
                else
                {
                    std::cerr << "Error AddConnection..." << std::endl;
                    break;
                }
            }
        }
    }

    int SockFd()
    {
        return _listensockfd->GetFd();
    }

private:
    u_int16_t _port;
    std::unique_ptr<Socket> _listensockfd;
};

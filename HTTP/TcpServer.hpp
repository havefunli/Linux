#include <thread>
#include "Socket.hpp"
#include <memory>
#include <functional>

using service_t = std::function<void(SockPtr)>;
using SockPtr = std::shared_ptr<Socket>;

class TcpServer
{
public:
    TcpServer() {}

    TcpServer(service_t func, uint16_t port, std::string ip = "0") 
        : _func(func)
        , _port(port)
        , _ip(ip)
        , _Ptr(std::make_shared<TcpSocket>())
    {
        _Ptr->BuildTcpListen(_port);
    }

    void Loop()
    {
        while(true)
        {
            SockAddHelper clientaddr;
            SockPtr newsock = _Ptr->AcceptConnect(clientaddr);

            // std::string request;
            // newsock->Recv(request);
            // std::cout << "Successful Recv..." << std::endl;
            // std::cout << request << std::endl;

            std::thread td(_func, newsock);
            td.detach(); 
        }
    }

    ~TcpServer() {}

private:
    SockPtr _Ptr;
    std::string _ip;
    uint16_t _port;

    service_t _func;
};
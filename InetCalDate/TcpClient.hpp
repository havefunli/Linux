#pragma once

#include "Socket.hpp"
#include <memory>
#include <functional>

using client_t = std::function<void(SockPtr&)>;
using SockPtr = std::shared_ptr<Socket>;

class TcpClient
{
public:
    TcpClient() {}

    TcpClient(client_t func, uint16_t port, std::string ip = "127.0.0.1") 
        : _func(func)
        , _port(port)
        , _ip(ip)
        , _Ptr(std::make_shared<TcpSocket>())
    {
        _Ptr->BuildTcpClient(_ip, _port);
    }

    void Loop()
    {
        std::cout << "Begin to execute..." << std::endl;
        while(true)
        {
            _func(_Ptr);
        }
    }

    ~TcpClient() {}

private:
    SockPtr _Ptr;
    std::string _ip;
    uint16_t _port;

    client_t _func;
};
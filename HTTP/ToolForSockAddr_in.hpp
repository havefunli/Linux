#pragma once

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <string>

// 该类再次封装了 struct sockaddr_in 
// 加上了常用的方法，便于调用

class SockAddHelper
{
private:
    void ToHost()
    {
        _ip = inet_ntoa(_local.sin_addr);
        _port = ntohs(_local.sin_port);
    }
    
public:
    SockAddHelper()
    {}

    SockAddHelper(std::string ip, uint16_t port)
        : _ip(ip)
        , _port(port)
    {
        _local.sin_family = AF_INET;
        _local.sin_port = htons(_port);
        _local.sin_addr.s_addr = inet_addr(_ip.c_str());
    }

    SockAddHelper(struct sockaddr_in local)
        : _local(local)
    {
        ToHost();
    }

    std::string GetIP()
    {
        return _ip;
    }

    uint16_t GetPort()
    {
        return _port;
    }

    struct sockaddr_in& GetAddr()
    {
        return _local;
    }

    socklen_t SizeofAdd()
    {
        return sizeof(_local);
    }

    struct sockaddr* ToSockaddr()
    {
        return (struct sockaddr*)&_local;
    }

    bool operator==(const SockAddHelper& addr)
    {
        if(addr._ip == _ip && addr._port == _port)
        {
            return true;
        }
        
        return false;
    }

private:
    struct sockaddr_in _local;
    std::string _ip;
    uint16_t _port;
};
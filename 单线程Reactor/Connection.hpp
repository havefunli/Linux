#pragma once
#include <iostream>
#include <string>
#include <functional>
#include "sys/epoll.h"
#include "ToolForSockAddr_in.hpp"

enum ConnType
{
    ListenConnection,
    NormalConnection
};


// 每一个文件描述符在底层就对应一个链接
// 单独为每一个文件描述符维护一个链接

// TODO 
class Reactor;
class Connection;
using handle_t = std::function<void(Connection*)>;

class Connection
{
public:
    Connection(int fd, uint32_t events)
        : _sockfd(fd)
        , _events(events)
        {}

    void SetPtr(Reactor* Ptr)
    {
        _R_Ptr = Ptr;
    }

    void SetAddr(SockAddHelper& addr)
    {
        _addr = addr;
    }

    void SetType(ConnType contype)
    {
        _contype = contype;
    }

    // 默认是 EPOLLET 模式
    void SetEvents(uint32_t events)
    {
        _events = events | EPOLLET;
    }

    Reactor* GetPtr()
    {
        return _R_Ptr;
    }

    void Register(handle_t recv, handle_t send, handle_t excep)
    {
        _handle_recver = recv;
        _handle_sender = send;
        _handle_excepter = excep;
    }

    int SockFd()
    {
        return _sockfd;
    }

    std::string& InBuff()
    {
        return _inbuffer;
    }

    std::string& OutBuff()
    {
        return _outbuffer;
    }

    bool DiscardOutBuff(int n)
    {
        if (n <= _outbuffer.size())
        {
            _outbuffer.erase(0, n);
            return true;
        }
        return false;
    }

private:
    int _sockfd;
    uint32_t _events;
    
    // 每个连接的读写缓冲区
    std::string _inbuffer;
    std::string _outbuffer;

    // 指向 Reactor 的指针
    Reactor* _R_Ptr;

    // 连接的类型
    ConnType _contype;

    // 地址
    SockAddHelper _addr;

public:
    // 每个连接对应处理相应事件的方式
    handle_t _handle_recver;
    handle_t _handle_sender;
    handle_t _handle_excepter;
};
#pragma once
#include <unordered_map>
#include "Socket.hpp"
#include "ToolForSockAddr_in.hpp"
#include "Epoll.hpp"
#include "Connection.hpp"


class Reactor
{
private: 
    bool IsAlive(int fd)
    {
        return _conn.find(fd) != _conn.end();
    }

    void PrintSockfd()
    {
        std::cout << "Managed fd: ";
        for (auto& pair : _conn)
        {
            std::cout << pair.first << " ";
        }
        std::cout << std::endl;
    }

public:
    Reactor()
        : _epoll(std::make_unique<Epoll>())
    {}

    bool AddConnection(int fd, uint32_t events, ConnType con_type, SockAddHelper addr)
    {
        // 先添加到 epoll 中管理
        if (!_epoll->AddEvent(fd, events)) return false;
        
        // 添加成功，创建一个新的连接
        Connection* conn = new Connection(fd, events);

        // 根据连接的类型设置相应的处理方法
        conn->SetType(con_type);
        if (con_type == ConnType::ListenConnection) // 监听连接
        {
            conn->Register(_onConnect, 0, 0);
        }
        else
        {
            conn->Register(_onRecv, _onSend, _onExcept);
        }

        // 设置地址和事件
        conn->SetAddr(addr);
        conn->SetEvents(events);

        // 让每一个连接指向 Reactor，便于后续添加操作
        conn->SetPtr(this);

        // 管理连接 
        _conn.insert(std::make_pair(fd, conn));

        std::cout << "Successful add new connection, new fd = " << fd << ", info: [" << addr.GetIP() << "][" << addr.GetPort() << "]" << std::endl;
        
        return true;
    }

    bool DelConnection(int fd)
    {
        if (IsAlive(fd))
        {
            
        }
    }

    // 单次事件就绪处理逻辑
    void LoopOnce()
    {
        int n = _epoll->Wait(revs, DefaultSize);
        if (n < 0) return;
        for (int i = 0; i < DefaultSize; i++)
        {
            int sockfd = revs[i].data.fd;
            uint32_t events = revs[i].events;

            // 事件可能出错了
            // 也可能是正常的读写事件
            // 为了更加方便的处理，将异常统一到读写一起处理
            if (events & EPOLLERR) events |= (EPOLLERR | EPOLLIN);
            if (events & EPOLLHUP) events |= ( EPOLLHUP | EPOLLIN);

            // 读事件
            if (events & EPOLLIN)
            {
                if (_conn[sockfd]->_handle_recver)
                {
                    _conn[sockfd]->_handle_recver(_conn[sockfd]);
                }
            }

            // 写事件, 并且需要额外判断该连接是否还存在
            if (!IsAlive(sockfd) && events & EPOLLOUT)
            {
                if (_conn[sockfd]->_handle_sender)
                {
                    _conn[sockfd]->_handle_sender(_conn[sockfd]);
                }
            }
        }
    }

    // 设置监听套接字处理逻辑处理
    void SetListenHanlder(handle_t onConnect)
    {
        _onConnect = onConnect;
    }

    // 设置普通套接字处理逻辑
    void SetNormalHanlder(handle_t onRecv, handle_t onSend, handle_t onExcept)
    {
        _onRecv = onRecv;
        _onSend = onSend;
        _onExcept = onExcept;
    }

    void EnableConnEvent(int fd, uint32_t events)
    {
        if (!IsAlive(fd))
        {
            std::cerr << "The fd " << fd << "is not exists..." << std::endl;
            return;
        }
        _conn[fd]->SetEvents(events);
        _epoll->ModEvent(fd, events);
    }

    // 事件派发
    void Dispatch()
    {
        while (true)
        {
            LoopOnce();
            PrintSockfd();
        }
    }

private:
    std::unique_ptr<Epoll> _epoll;
    std::unordered_map<int, Connection*> _conn;

    struct epoll_event revs[DefaultSize];

    // 监听套接字处理
    handle_t _onConnect;
    // 处理读写异常
    handle_t _onRecv;
    handle_t _onSend;
    handle_t _onExcept;
};
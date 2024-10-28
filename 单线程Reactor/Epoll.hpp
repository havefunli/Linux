#pragma once 
#include <iostream>
#include <sys/epoll.h>


const int DefaultSize = 1024;

class Epoll
{
private:
    struct epoll_event CreateEpollEvent(int fd, uint32_t events)
    {
        struct epoll_event ev;
        ev.data.fd = fd;
        ev.events = EPOLLET | events;

        return ev;
    }

    bool ModEventHelper(int fd, uint32_t events, int oper)
    {
        struct epoll_event ev = CreateEpollEvent(fd, events);
        int n = epoll_ctl(_epfd, oper, fd, &ev);
        if (n < 0)
        {
            perror("epoll_ctl");
            return false;
        }
        return true;
    }

public:
    Epoll()
    {
        _epfd = epoll_create(DefaultSize);
        if (_epfd < 0)
        {
            perror("epoll_create");
            exit(1);
        }
        std::cout << "Successful create epoll..." << std::endl;
    }

    bool AddEvent(int fd, uint32_t events)
    {
        return ModEventHelper(fd, events, EPOLL_CTL_ADD);
    }

    bool ModEvent(int fd, uint32_t events)
    {
        return ModEventHelper(fd, events, EPOLL_CTL_MOD);
    }

    bool DelEvent(int fd)
    {
        return ModEventHelper(fd, 0, EPOLL_CTL_DEL);
    }

    int Wait(struct epoll_event rev[], int num)
    {
        int n = epoll_wait(_epfd, rev, num, -1);
        if (n < 0)
        {
            perror("epoll_wait");
        }
        
        return n;        
    }

private:
    int _epfd;
};





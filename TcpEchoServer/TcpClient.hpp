#pragma once

#include "SockaddrHelper.hpp"

#include <string>
#include <cstring>
#include <iostream>

#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>


class TcpClient
{
public:
    TcpClient(std::string ip, uint16_t port)
        : _ip(ip)
        , _port(port)
    {
        std::cout << "Create TcpClient..." << std::endl; 
    }

    void Init()
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(_sockfd < 0)
        {
            perror("socket:");
        }
        std::cout << "Create socket..." << std::endl;
    
        SockaddrHelper sockhelper(_ip, _port);
        int n = connect(_sockfd, sockhelper.ToSockaddr(), sockhelper.SizeofAdd());
        if(n < 0)
        {
            perror("connect:");
            exit(0);
        }
        std::cout << "Successful connect..." << std::endl;
        
        std::cout << "Successful init..." << std::endl;
    }

    void Start()
    {
        while (true)
        {
            std::string msg;
            std::cout << "Please Enter# ";
            std::getline(std::cin, msg);

            write(_sockfd, msg.c_str(), msg.size());
        }
    }

private:
    int _sockfd;
    std::string _ip;
    uint16_t _port;
};
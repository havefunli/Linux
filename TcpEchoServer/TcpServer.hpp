#pragma once

#include "SockaddrHelper.hpp"

#include <string>
#include <iostream>

#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int sockfd = -1;

class TcpServer
{
public:
    TcpServer(uint16_t port, std::string ip = "0")
        : _port(port)
        , _ip(ip)
    {
        std::cout << "Create TcpServer..." << std::endl; 
    }

    void Init()
    {
        _listensockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(_listensockfd < 0)
        {
            perror("socket:"); 
            exit(0);
        }
        std::cout << "Successful create socket!!!" << std::endl;

        // 指定地址
        SockaddrHelper sockHelper(_ip, _port);
        // 绑定
        if(bind(_listensockfd, sockHelper.ToSockaddr(), sockHelper.SizeofAdd()) < 0)
        {
            perror("bind");
            exit(0);
        }
        std::cout << "Successful bind..." << std::endl;

        // Tcp 面向连接，需要监听
        if (listen(_listensockfd, 3) < 0) 
        {  
            perror("listen");  
            exit(0);
        }
        std::cout << "Successful listen..." << std::endl;

        std::cout << "Successful init..." << std::endl;
    }

    void Service(int sockfd, SockaddrHelper client)
    {
        while(true)
        {
            char inBuff[1024];
            ssize_t n = read(sockfd, inBuff, sizeof(inBuff) - 1);
            if(n > 0)
            {
                inBuff[n] = '0';
                std::cout << "Successful load msg..." << std::endl;

                std::string result = "[" + client.GetIP() + ":" + std::to_string(client.GetPort()) + "]" + "# " + inBuff;
                std::cout << result << std::endl;
            }
            else if(n == 0)
            {
                std:: cout << "Client quit..." << std::endl;
                break;
            }
            else
            {
                std::cerr << "read error!!!" << std::endl;
                break;
            }
        }
    }

    void Loop()
    {
        // 忽略子进程的信号
        signal(SIGCHLD, SIG_IGN);
        
        while(true)
        {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);

            int sockfd = accept(_listensockfd, (struct sockaddr*)&client, &len);
            if(sockfd < 0)
            {
                std::cerr << "accept error!!!" << std::endl;
                continue;
            }
            std::cout << "Successful accept..." << std::endl;
        
            // 利用多进程处理
            pid_t id = fork();
            // child
            if(id == 0)
            {
                close(_listensockfd);
                
                std::cout << "child process begin..." << std::endl;
                Service(sockfd, SockaddrHelper(client));

                std::cout << "child exit..." << std::endl;
                exit(0);
            }
            // father
            else if(id > 0)
            {
                close(sockfd);
            }
            else
            {
                std::cerr << "fork error!!!" << std::endl;
                break;
            }
        }
    }

private:
    int _listensockfd; // 监听
    uint16_t _port;
    std::string _ip;
};
#pragma once

#include "ToolForSockAddr_in.hpp"

#include <sys/types.h>
#include <sys/socket.h>

#include <unistd.h>
#include <memory>
#include <cstdio>
#include <iostream>
#include <string>


class Socket;

const int DefaultFd = -1;
using SockPtr = std::shared_ptr<Socket>;

class Socket
{
public:
    virtual void CreateSocket() = 0; // 创建套接字文件
    virtual void BindAddress(uint16_t, const std::string& ip = "0") = 0; // 绑定地址
    virtual void CreateListen(int backlog = 5) = 0; // 监听连接
    virtual void Connect(const std::string&, uint16_t) = 0; // 连接
    virtual SockPtr AcceptConnect(SockAddHelper&) = 0; // 接受连接
    virtual bool Recv(std::string& out) = 0; // 接收消息
    virtual ssize_t Send(const std::string&) = 0; // 发送消息
    virtual int GetFd() = 0; // 获取文件描述符

public:
    void BuildTcpListen(uint16_t PORT, const std::string& ip = "0", int backlog = 5)
    {
        CreateSocket();
        BindAddress(PORT, ip);
        CreateListen(backlog);
    }

    void BuildTcpClient(const std::string& ip, uint16_t PORT)
    {
        CreateSocket();
        Connect(ip, PORT);
    }
};


class TcpSocket : public Socket
{
public: 
    TcpSocket() {}

    TcpSocket(int fd) 
        : _sockfd(fd)    
    {}

    void CreateSocket()
    {
        _sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if(_sockfd < 0) 
        {
            perror("socket:");
            exit(1);
        }

        std::cout << "Successful create socket..." << std::endl;
    }

    void BindAddress(uint16_t port, const std::string& ip) override
    {
        SockAddHelper sockhelper(ip, port);
        if(bind(_sockfd, sockhelper.ToSockaddr(), sockhelper.SizeofAdd()) < 0) 
        {
            perror("bind:");
            exit(1);
        }

        std::cout << "Successful bindaddress..." << std::endl;
    }
    
    void CreateListen(int backlog) override
    {
        if(listen(_sockfd, backlog) < 0) 
        {
            perror("listen");
            exit(1);
        }

        std::cout << "Successful listening..." << std::endl;
    }

    SockPtr AcceptConnect(SockAddHelper& clientaddr) override
    {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);

        int fd = accept(_sockfd, (sockaddr*)&client, &len);
        if(fd < 0) perror("accept:");

        clientaddr = SockAddHelper(client);
        std::cout << "Successful accept..." << std::endl;
        return std::make_shared<TcpSocket>(fd);
    }

    void Connect(const std::string& serip, uint16_t serport) override
    {
        SockAddHelper serveraddr(serip, serport);
        int n = connect(_sockfd, serveraddr.ToSockaddr(), serveraddr.SizeofAdd());
        if(n < 0)
        {
            perror("connect:");
            exit(1);
        }

        std::cout << "Successful connect..." << std::endl;
    }

    bool Recv(std::string& out) override
    {
        char OutBuff[1024];
        ssize_t n = recv(_sockfd, OutBuff, sizeof(OutBuff) - 1, 0);
        if(n > 0)
        {
            OutBuff[n] = '0';
            out = OutBuff;
            return true;
        }
        else if(n < 0)
        {
            perror("recv:");
            return false;
        }
        else
        {
            return false;
        }
    }

    ssize_t Send(const std::string& in) override
    {
        int n = send(_sockfd, in.c_str(), in.size(), 0);
        return n;
    }

    int GetFd() override
    {
        return _sockfd;
    }

private:
    int _sockfd;
};

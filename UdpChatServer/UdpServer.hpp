#include "nocopy.hpp"
#include "ToolForSockAddr_in.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <functional>

#include <arpa/inet.h> 
#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h>

const int GSockfd = -1;
using func_t = std::function<std::string(const std::string&)>;

class UdpServer : public nocopy
{
public:
    UdpServer(func_t func, uint16_t localport, std::string localip = "0")
        : _func(func)
        , _sockfd(GSockfd)
        , _localip(localip)
        , _localport(localport)
        , _isRunning(false)
    {}

    // 初始化套接字等信息
    void Init()
    {
        // 1. 创建套接字文件
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if(-1 == _sockfd) 
        { 
            perror("socket:");
            exit(1); 
        }

        TFSA_in local(_localip, _localport);
        if(bind(_sockfd, local.ToSockaddr(), local.SizeofAdd()) < 0)
        {
            perror("bind");
        }

        std::cout << "Server successfull init...!" << std::endl;
    }

    void Start()
    {
        _isRunning = true;
        char inBuffer[1024];

        while(1)
        {
            struct sockaddr_in client_addr;
            socklen_t len = sizeof(client_addr);
            ssize_t n = recvfrom(_sockfd, inBuffer, sizeof(inBuffer) - 1, 0, 
                                (struct sockaddr*)&client_addr, &len);
            if(n > 0)
            {
                inBuffer[n] = 0;
                TFSA_in client(client_addr);
                printf("[%s:%hu]# %s\n", client.GetIP().c_str(), client.GetPort(), inBuffer);
            
                // 回信息
                std::string result = _func(inBuffer);
                ssize_t n = sendto(_sockfd, result.c_str(), result.size(), 0, client.ToSockaddr(), client.SizeofAdd());
                if(n < 0)
                {
                    perror("sendto:");
                }
            }
            else
            {
                perror("recvfrom");
            }
        }
    }

    ~UdpServer()
    {
        if(_sockfd > 0)
        {
            close(_sockfd);
        }
    }

private:
    int _sockfd;
    uint16_t _localport;
    std::string _localip;
    bool _isRunning;

    func_t _func;// 具体的执行逻辑
};
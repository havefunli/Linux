#include "nocopy.hpp"
#include "ToolForSockAddr_in.hpp"

#include <iostream>
#include <string>
#include <cstring>
#include <stdio.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

const int GSockfd = -1;

class UdpClient
{
public:
    UdpClient(std::string serverip, uint16_t serverport)
        : _isRunning(false)
        ,  _sockfd(GSockfd)
        , _serverip(serverip)
        , _serverport(serverport)
    {}

    void Init()
    {
        // 初始化套接字文件
        if ((_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        {
            perror("socket:");
            exit(1);
        }
        std::cout << "Client successfull init...!" << std::endl;
    }

    void Start()
    {
        // client 端口不需要用户指定，OS 自动分配
        // client 不需要显示的绑定自己的端口和 IP
        // 在首次向服务器发送信息时，会自动绑定
        _isRunning = true;
        TFSA_in server(_serverip, _serverport);

        while(1)
        {
            std::string txt;
            std::cout << "Please Enter# ";
            std::getline(std::cin, txt);

            int n = sendto(_sockfd, txt.c_str(), txt.size(), 0, server.ToSockaddr(), server.SizeofAdd());

            char inBuffer[1024];
            if(n > 0)
            {
                struct sockaddr_in server_addr;
                socklen_t len = sizeof(server_addr);
                ssize_t n = recvfrom(_sockfd, inBuffer, sizeof(inBuffer) - 1, 0, (struct sockaddr*)&server_addr, &len);
                if(n > 0)
                {
                    inBuffer[n] = 0;
                    TFSA_in server(server_addr);
                    printf("[%s:%hu]# %s\n", server.GetIP().c_str(), server.GetPort(), inBuffer);
                }
                else
                {
                    perror("recvfrom:");
                }
            }
            else
            {
                perror("sendto");
            }
        }
    }

private:
    bool _isRunning;
    int _sockfd;
    std::string _serverip;
    uint16_t _serverport;
};
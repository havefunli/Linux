#include <cstdio>
#include <iostream>
#include <string>

#include <sys/types.h>          
#include <sys/socket.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cout << "Usage: ./server port" << std::endl;
        exit(1);
    }

    // 获取端口号
    uint16_t PORT = std::stoi(argv[1]);

    // 初始化结构体字段
    struct sockaddr_in address;
    address.sin_family = AF_INET; // 网络通信
    address.sin_addr.s_addr = INADDR_ANY; // 将点分十进制的字符串改为长整型 127.0.0.1 => 0x7F000001
    address.sin_port = htons(PORT); // 将端口号转化位网络字节序
    std::cout << "Successful init sockaddr_in..." << std::endl;

    // AF_INET 代表网络通信
    // SOCK_DGRAM 代表 UDP 协议
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sockfd < 0)
    {
        perror("socket:");
    }
    std::cout << "Successful create sockfd..." << std::endl;

    // 绑定socket到端口 
    int n = bind(sockfd, (struct sockaddr*)(&address), sizeof(address));
    if(n < 0)
    {
        perror("bind:");
        exit(0);
    }
    std::cout << "Successful bind..." << std::endl;

    // 不断地接受客户端的信息
    char msg[1024];
    struct sockaddr_in clientAddr;
    socklen_t len = sizeof(clientAddr);
    while(true)
    {
        int n = recvfrom(sockfd, msg, sizeof(msg), 0, (struct sockaddr*)(&clientAddr), &len); // 接收消息
        if(n < 0)
        {
            perror("recvfrom:");
            continue;
        }
        else if(n == 0)
        {
            std::cout << "Client Quit..." << std::endl;
            exit(0);
        }

        msg[n] = '\0';
        printf("[%s:%d]# %s\n", inet_ntoa(clientAddr.sin_addr),
                                ntohs(clientAddr.sin_port),
                                msg);
    }

    return 0;
}

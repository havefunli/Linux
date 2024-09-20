#include <unistd.h>
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
    address.sin_addr.s_addr = INADDR_ANY; // 任意地址皆可
    address.sin_port = htons(PORT); // 将端口号转化位网络字节序
    std::cout << "Successful init sockaddr_in..." << std::endl;

    // AF_INET 代表网络通信
    // SOCK_STREAM 代表 TCP 协议
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    // 监听
    n = listen(sockfd, 3);
    if(n < 0)
    {
        perror("listen:");
        exit(1);
    }
    std::cout << "Successful listening..." << std::endl;

    // 连接
    struct sockaddr_in ClientAddress; // 用于接收客户端的信息
    socklen_t len = sizeof(ClientAddress);
    int newsockfd = accept(sockfd, (sockaddr*)(&ClientAddress), &len);
    if(newsockfd < 0)
    {
        perror("accept:");
        exit(1);
    }
    std::cout << "Successful accept..." << std::endl;

    // 不断地接受客户端的信息
    char msg[1024];
    while(true)
    {
        int n = recv(newsockfd, msg, sizeof(msg), 0);
        if(n < 0)
        {
            perror("read");
            continue;
        }
        else if(n == 0)
        {
            std::cout << "Client Quit..." << std::endl;
            exit(0);
        }

        msg[n] = '\0';
        printf("[%s:%d]# %s\n", inet_ntoa(ClientAddress.sin_addr),
                                ntohs(ClientAddress.sin_port),
                                msg);
    }

    return 0;
}

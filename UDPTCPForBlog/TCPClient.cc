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
    if(argc != 3)
    {
        std::cout << "Usage: ./server ip port" << std::endl;
        exit(1);
    }

    // 获取 IP
    std::string IP = argv[1];
    // 获取端口号
    uint16_t PORT = std::stoi(argv[2]);

    // 初始化结构体字段
    struct sockaddr_in address;
    address.sin_family = AF_INET; // 网络通信
    address.sin_addr.s_addr = inet_addr(IP.c_str()); // 将点分十进制的字符串改为长整型 127.0.0.1 => 0x7F000001
    address.sin_port = htons(PORT); // 将端口号转化位网络字节序

    // AF_INET 代表网络通信
    // SOCK_STREAM 代表 TCP 协议
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket:");
    }
    
    // 连接请求
    int n = connect(sockfd, (struct sockaddr*)(&address), sizeof(address));
    if(n < 0)
    {
        perror("connect:");
        exit(1);
    }
    std::cout << "Successful connect..." << std::endl;

    // 不断地向服务端发送信息
    std::string msg;
    while(true)
    {
        std::cout << "Please Enter# ";
        std::cin >> msg;

        ssize_t n = send(sockfd, msg.c_str(), msg.size(), 0);
        if(n < 0)
        {
            perror("send:");
            continue;
        }
    }

    return 0;
}
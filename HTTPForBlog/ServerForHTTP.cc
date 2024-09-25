#include <cstdio>
#include <iostream>
#include <string>
#include <fstream>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>          
#include <sys/socket.h>

std::string GetIndex()
{
    std::ifstream in("./index.html");
    if(!in.is_open())
    {
        perror("File does not exist:");
        exit(1);
    }

    in.seekg(0, in.end);
    int size = in.tellg();
    in.seekg(0, in.beg);

    std::string content(size, 0);
    in.read((char*)content.c_str(), size); 
    in.close();

    return content;
}

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: ./server PORT" << std::endl;
    }

    uint16_t PORT = std::stoi(argv[1]);

    // 创建套接字文件
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0)
    {
        perror("socket:");
        exit(1);
    }
    std::cout << "Successful create sockfd..." << std::endl;

    // 绑定
    struct sockaddr_in Address;
    Address.sin_family = AF_INET;
    Address.sin_addr.s_addr = INADDR_ANY;
    Address.sin_port = htons(PORT);
    int n = bind(sockfd, (struct sockaddr*)&Address, sizeof(Address));
    if(n < 0)
    {
        perror("bind::");
        exit(1);
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

    // 等待接收连接
    struct sockaddr_in ClientAddress;
    socklen_t len = sizeof(ClientAddress);
    int newfd = accept(sockfd,  (struct sockaddr*)&ClientAddress, &len);
    if(newfd < 0)
    {
        perror("accept:");
        exit(1);
    }
    std::cout << "Successful accept..." << std::endl;

    std::string Content = GetIndex();
    while(true)
    {
        std::string response;
        response += "HTTP/1.0 200 OK\n";
        response += "Content-Length:" + std::to_string(Content.size()) + '\n';
        response += '\n'+ Content;

        send(newfd, response.c_str(), response.size(), 0);
    }

    return 0;
}
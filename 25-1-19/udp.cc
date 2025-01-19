#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>

#define MAXSIZE 1024
#define PORT 8888

char msg[MAXSIZE] = {'\0'};

int main () {
    /// 创建套接字
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("socket");
        return -1;
    }
    
    // 绑定信息
    struct sockaddr_in host;
    host.sin_family = AF_INET;
    host.sin_addr.s_addr = INADDR_ANY;
    host.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr*)(&host), sizeof(host)) < 0) {
        perror("bind");
        return -1;
    }

    while (true) {
        struct sockaddr_in client;
        socklen_t len = sizeof(client);

        int n = recvfrom(sockfd, (void*)msg, MAXSIZE, 0, (struct sockaddr*)(&client), &len);
        if (n < 0) {
            perror("recvfrom");
            return -1;
        } else {
            msg[n] = '\0';
            printf("client ip = %s, port = %d, msg = %s\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port), msg);
        }
    }
    return 0;
}
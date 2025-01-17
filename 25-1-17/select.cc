#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/select.h>
#include <spdlog/spdlog.h>

const char* msg = "Hello, World!";

int main () {
    // 创建一个套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        spdlog::error("create sockfd error");
        return -1;
    }

    // 绑定地址
    struct sockaddr_in host;
    host.sin_family = AF_INET;
    host.sin_addr.s_addr = INADDR_ANY;
    host.sin_port = htons(8888);
    if (bind(sockfd, (struct sockaddr*)(&host), sizeof(host)) < 0) {
        spdlog::error("bind error");
        return -1;
    }

    // 开始监听
    if (listen(sockfd, 3) < 0) {
        perror(":listen error");
        return -1;
    }
    

    spdlog::info("The server on port = 8888, begin to work");
    
    fd_set rfds;
    // 清空
    FD_ZERO(&rfds);
    // 设置
    FD_SET(sockfd, &rfds);
    while (true) {
        // 规定时间
        struct timeval timeout;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        int n = select(sockfd + 1, &rfds, 0, 0, &timeout);
        if (n < 0) {
            spdlog::error("select error");
            return -1;
        } else if (n == 0) {
            spdlog::info("没有任何事件触发");
        } else {
            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int newfd = accept(sockfd, (struct sockaddr*)(&client), &len);
            spdlog::info("The client info, ip = {}, port = {}", inet_ntoa(client.sin_addr), ntohl(client.sin_port));
            write(newfd, msg, sizeof(msg));
            close(newfd);
        }

        FD_ZERO(&rfds);
        FD_SET(sockfd, &rfds);
    }

    return 0;
}
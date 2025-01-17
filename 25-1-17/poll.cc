#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/poll.h>
#include <spdlog/spdlog.h>

const char *msg = "Hello, World";

int main()
{
    // 创建一个套接字
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        spdlog::error("create sockfd error");
        return -1;
    }

    // 绑定地址
    struct sockaddr_in host;
    host.sin_family = AF_INET;
    host.sin_addr.s_addr = INADDR_ANY;
    host.sin_port = htons(8888);
    if (bind(sockfd, (struct sockaddr *)(&host), sizeof(host)) < 0)
    {
        spdlog::error("bind error");
        return -1;
    }

    // 开始监听
    if (listen(sockfd, 3) < 0)
    {
        perror(":listen error");
        return -1;
    }

    while (true) {
        pollfd rfd[1];
        rfd[0].fd = sockfd;
        rfd[0].events = POLLIN;
        int n = poll(rfd, sockfd + 1, 5000);

        if (n < 0) {
            spdlog::error("poll error");
            return -1;
        } else if (n == 0) {
            spdlog::info("Nothing happen");
        } else {
            int fd = rfd[0].fd;

            struct sockaddr_in client;
            socklen_t len = sizeof(client);
            int newfd = accept(fd, (struct sockaddr*)(&client), &len);

            spdlog::info("Client info, ip = {}, port = {}", inet_ntoa(client.sin_addr), ntohl(client.sin_port)); 
            write(newfd, msg, sizeof(msg));

            close(newfd);  
        }
    }

    return 0;
}
// 将标准输入的文件描述符修改为非阻塞
#include <unistd.h>
#include <fcntl.h>
#include <iostream>

int main() {

    // 获取文件状态
    int flags = 0;
    flags = fcntl(0, F_GETFL);
    if (flags < 0) {
        perror("fcntl");
        return -1;
    }

    // 设置非阻塞状态
    flags |= O_NONBLOCK;
    if (fcntl(0, F_SETFL, flags) < 0) {
        perror("fcntl");
        return -1;
    }

    char msg[1024];
    while (true) {
        int n = read(0, msg, sizeof(msg));
        if (n > 0) {
            msg[n] = '\0';
            std::cout << msg << std::endl;
        } else {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
                std::cout << "There is nothing input" << std::endl;
            } else {
                perror("read");
                return -1;
            }
        }
    }

    return 0;
}
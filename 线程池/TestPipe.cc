// 本程序使用管道使父子进程之间单项通信

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstring>
#include <string>


#define MAXSIZE 1000


int main(){
    int pipefd[2];
    int ret = pipe(pipefd); // 创建一个管道，1 代表写，0 代表读
    // 判断是否创建成功
    if(ret == -1){
        std::cerr << "errno: " << errno << ", errstring: " << strerror(errno) << std::endl;
        return 1;
    }
    
    // 创建一个进程
    int rid = fork(); 
    if(rid == -1){
        std::cerr << "errno: " << errno << ", errstring: " << strerror(errno) << std::endl;
        return 1;
    }
    // child process
    else if(rid == 0){
        close(pipefd[0]); // 关闭子进程的读，让子进程写入
        int wfd = pipefd[1]; 
        std::string msg = "I am child, i dont have an idea!\n";

        while(1){
            ssize_t ret_w = write(wfd, msg.c_str(), msg.size());
            if(ret_w == -1){
                std::cerr << "errno: " << errno << ", errstring: " << strerror(errno) << std::endl;
                exit(1);
            }
            sleep(1);
        }

        close(pipefd[1]);
    }
    // father process
    close(pipefd[1]); // 关闭父进程的写，让父进程读
    int rfd = pipefd[0];

    while(1){
        char buf[MAXSIZE];
        ssize_t ret_r = read(rfd, buf, sizeof(buf) - 1);
        if(ret_r > 0){
            buf[ret_r] = '\0';

            std::cout << buf << std::endl;
        }
    }

    int status = 1;
    pid_t n = waitpid(rid, &status, 0);

    close(pipefd[0]);

    return 0;
}
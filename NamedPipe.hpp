#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <cstring>
#include <string>
#include <cerrno>

#define READ O_RDONLY
#define WRITE O_WRONLY

#define MAXSIZE 1024

// 默认 fifo 文件位置
const std::string _Fifo_Path = "./fifo_temp";

// 标识身份
enum ID
{
    Creater,
    USER
};

class NamedPipe
{
private:
    void CreatNamedPipe()
    {
        std::cout << "Create a fifo!" << std::endl;
        int ret = mkfifo(_Fifo_Path.c_str(), 0666);
        if (ret == -1)
        {
            perror("mkfifo");
        }
    }

    bool OpenNamedPipe(int mode)
    {
        int fd = open(_Fifo_Path.c_str(), mode);
        if (fd == -1)
        {
            perror("open");
            return false;
        }

        _fd = fd;
        return true;
    }

public:
    NamedPipe(ID id)
        : _id(id)
    {
        // 如果是创建者，则创建文件
        if (_id == Creater)
        {
            CreatNamedPipe();
        }
    }

    // 标志用于读
    bool OpenForRead()
    {
        return OpenNamedPipe(READ);
    }

    // 标志用于写
    bool OpenForWrite()
    {
        return OpenNamedPipe(WRITE);
    }

    // 发送信息
    void send(const std::string &in)
    {
        
        ssize_t ret = write(_fd, in.c_str(), in.size());
        if(ret == -1)
        {
            perror("write");
        }
    }

    int Receive(std::string &msg)
    {   
        char buf[MAXSIZE];
        ssize_t n = read(_fd, buf, MAXSIZE - 1);
        if(n == -1)
        {
            perror("read");
            return n;
        }
        else
        {
            buf[n] = '\0';
        }

        msg = buf;
        return n;
    }

    ~NamedPipe()
    {
        // 如果是创建者，则删除文件
        if (_id == Creater)
        {
            int ret = unlink(_Fifo_Path.c_str());
            if(ret == -1)
            {
                perror("unlink");
            }
            std::cout << "creater free named pipe" << std::endl;
        }

        if (_fd != -1)
        {
            int ret = close(_fd);
        }
    }

private:
    int _fd = -1;
    ID _id = USER;
};
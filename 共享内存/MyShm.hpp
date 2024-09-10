#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

#include <unistd.h>
#include <iostream>
#include <cerrno>
#include <string>
#include <cstdlib>
#include <cstring>

#define CREATER 1
#define USER 2

#define MAXSIZE 4096

const std::string _WorkPath = "/home/ubuntu/8_12";
const int _Proj_id = 135;

class Shm
{
private:
    // 获取键值
    key_t GetCommenKey()
    {
        key_t key = ftok(_pathname.c_str(), _proj_id);
        if (key == -1)
        {
            perror("ftok");
        }

        return key;
    }

    // 获取内存标识符
    int GetShmID()
    {
        int shmid = -1;
        if (_id == USER)
        {
            shmid = shmget(_key, MAXSIZE, IPC_CREAT | IPC_EXCL | 0666);
            if (shmid < 0)
            {
                perror("shmget for creater");
            }

            std::cout << "CREATER create a share memory!" << std::endl;
        }

        else
        {
            shmid = shmget(_key, MAXSIZE, IPC_CREAT | 0666);
            if (shmid < 0)
            {
                perror("shmget for user");
            }

            std::cout << "USER get the share memory!" << std::endl;
        }

        return shmid;
    }

    // 将共享内存映射到进程地址上
    void *AttachShm()
    {
        void *addrshm = shmat(_shmid, nullptr, 0);
        if(addrshm == nullptr)
        {
            perror("addrshm");
        }

        return addrshm;
    }

    // 释放共享内存
    bool ShmctlHelper()
    {
        int n = shmctl(_shmid, IPC_RMID, nullptr);
        if(n == -1)
        {
            perror("shmctl");
            return false;
        }

        std::cout << "Detach the share memory." << std::endl;
        return true;
    }

public:
    Shm(const std::string &pathname, int proj_id, int id)
        : _pathname(pathname), _proj_id(proj_id), _id(id)
    {
        _key = GetCommenKey();

        _shmid = GetShmID();

        _addrshm = AttachShm();

        Clear();
    }

    ~Shm()
    {
        if(_id == USER)
        {
            bool n = ShmctlHelper();
        }

        _addrshm = nullptr;
    }
    
    void *GetShmAddr()
    {
        return _addrshm;
    }

    void Clear()
    {
        if(_addrshm)
        {
            memset(_addrshm, 0, MAXSIZE);
        }
    }

private:
    key_t _key;
    int _shmid;

    std::string _pathname;
    int _proj_id;

    int _id;

    void *_addrshm;
};
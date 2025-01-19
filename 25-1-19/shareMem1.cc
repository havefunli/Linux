#include <sys/ipc.h>
#include <sys/shm.h>
#include <iostream>

#define MAXSIZE 1024

int main () {
    // 创建一个 key 值
    key_t key = ftok("shareMem1.cc", 't');
    
    // 创建共享内存
    int shmid = shmget(key, MAXSIZE, 0666 | IPC_CREAT);
    if (shmid < 0 && errno != EEXIST) {
        perror("shmget");
        return -1;
    }
    std::cout << shmid << std::endl;
    // 挂载到进程上
    void *addr = shmat(shmid, NULL, 0);
    if (addr == (void*)(-1)) {
        perror("shmat");
        return -1;
    }

    sprintf((char*)addr, "i am ok!");

    // 去除
    shmdt(addr);

    return 0;
}
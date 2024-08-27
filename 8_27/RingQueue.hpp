#pragma once

#include <iostream>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

const size_t DefalutCap = 5;

template <class T>
class RingQueue
{
private:

    // 请求资源
    void P(sem_t *sem)
    {
        sem_wait(sem);
    }

    // 释放资源
    void V(sem_t *sem)
    {
        sem_post(sem);
    }

public:
    RingQueue(size_t MaxCap = DefalutCap)
        : _MaxCap(MaxCap)
        , _RingQueue(MaxCap)
        , _Start(0)
        , _End(0)
    {
        // 初始化信号量，数据最开始为 0，空间为满
        sem_init(&_DataSem, 0, 0);
        sem_init(&_SpaceSem, 0, _MaxCap);

        // 初始化锁
        pthread_mutex_init(&_PMutex, nullptr);
        pthread_mutex_init(&_CMutex, nullptr);
    }

    // 生产者放入数据
    void push(const T &in)
    {
        // 判断是否还有剩余的空间
        P(&_SpaceSem);
        
        // 访问临界区需要上锁
        pthread_mutex_lock(&_PMutex);
        
        _RingQueue[_End] = in;
        // 更新下标
        _End++;
        _End %= _MaxCap;
        
        pthread_mutex_unlock(&_PMutex);
        V(&_DataSem); // 增加数据资源
    }

    // 消费者获取数据
    void pop(T &val)
    {
        // 判断是否还有剩余的数据
        P(&_DataSem);
        
        // 上锁
        pthread_mutex_lock(&_CMutex);

        val = _RingQueue[_Start];
        // 更新下标
        _Start++;
        _Start %= _MaxCap;

        pthread_mutex_unlock(&_CMutex);
        V(&_SpaceSem);
    }

    ~RingQueue()
    {
        // 释放信号量
        sem_destroy(&_DataSem);
        sem_destroy(&_SpaceSem);

        // 释放锁
        pthread_mutex_destroy(&_PMutex);
        pthread_mutex_destroy(&_CMutex);
    }

private:
    // 实现的容器以及最大容量
    std::vector<T> _RingQueue;
    size_t _MaxCap;

    // 指向环形队列的起始和终止位置
    size_t _Start;
    size_t _End;

    // 为消费者和生产者准备的信号量
    sem_t _DataSem;
    sem_t _SpaceSem;

    // 访问临界区需要锁
    pthread_mutex_t _PMutex;
    pthread_mutex_t _CMutex;
};
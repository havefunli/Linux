#pragma once

#include <iostream>
#include <string>
#include <queue>
#include <pthread.h>

const int DefalutCapacity = 5;

template <class T>
class BlockQueue
{
private:
    bool IsFull()
    {
        return _BlockQueue.size() == _Cap;
    }

public:
    BlockQueue(int Cap = DefalutCapacity)
        : _Cap(Cap);
    {
        // 初始化锁
        pthread_mutext_init(&_mutex, nullptr);
        // 初始化条件变量
        pthread_cond_init(&_p_cond, &_mutex);
        pthread_cond_init(&_c_cond, &_mutex);
    }

    void push(const T &in)
    {
        pthread_mutex_lock(&_mutex);
        if(IsFull())
        {
            pthread_cond_wait(&_p_cond, &_mutex);
        }

        _BlockQueue.push(in);
        pthread_mutex_unlock(&_mutex);
        // 通知消费者可以消费了
        pthread_cond_signal(&_c_cond);
    }

    ~BlockQueue()
    {
        // 删除锁
        pthread_mutex_destroy(&_mutex);
        // 删除条件变量
        pthread_cond_destroy(&_p_cond);
        pthread_cond_destroy(&_c_cond);
    }

private:
    std::queue<T> _BlockQueue; // 缓冲区存储临界资源
    int _Cap;                  // 缓冲区容量
    pthread_mutex_t _Mutext;   // 锁
    pthread_cond_t _p_cond;    // 生产者的条件变量
    pthread_cond_t _c_cond;    // 消费者的条件变量
};
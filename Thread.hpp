#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>

// 线程的状态
enum Status
{
    Running,
    Sleeping
};

struct ThreadData
{
    ThreadData(std::string name, pthread_mutex_t *lock)
        : _name(name)
        , _lock(lock)
        {}

    pthread_mutex_t *_lock;
    std::string _name;
};

class Thread
{
    // 函数指针，指向线程执行的函数
    // typedef void (*func_t)(const std::string &name);

    typedef void (*func_t)(ThreadData *td);

private: 

static void *ThreadRoutine(void *arg)
{
    Thread *self = static_cast<Thread*>(arg); // 安全的类型转换    
    std::cout << self->_name << " start Running !!!" << std::endl;

    self->_status = Running;
    // 执行相关函数
    self->_func(self->_td);
    self->_status = Sleeping;

    return nullptr;
}

public:
    Thread(const std::string &name, func_t func, ThreadData *td)
        : _name(name)
        , _func(func)
        , _td(td)
    {
        _status = Sleeping;
        std::cout << "Successfully Created " << _name << "." << std::endl;
    }

    bool Start()
    {
        if(pthread_create(&_tid, nullptr, ThreadRoutine, this) != 0) { return false; }
        return true;
    }

    bool Stop()
    {
        if(_status == Running)
        {
            if(pthread_cancel(_tid) != 0){ return false; }
            _status = Sleeping;

            std::cout << _name << " Stoped!" << std::endl;
            return true;
        }
        
        return false;
    }

    bool Join()
    {
        if(pthread_join(_tid, nullptr) != 0) { return false; }
        delete _td;

        std::cout << _name << " Joined!" << std::endl;
        return true;
    }
    
    ~Thread()
    {
        
    }

private:
    std::string _name;
    pthread_t _tid;
    func_t _func;
    Status _status;
    ThreadData *_td;
};
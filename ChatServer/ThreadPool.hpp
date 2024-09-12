#include "Thread.hpp"

#include <unistd.h>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <pthread.h>

size_t DefaultNums = 5;

// 该线程池是单例模式
// 传递给线程池的任务需要经过处理，一律无参
class ThreadPool
{
private:

    bool IsEmpty()
    {
        return _task_queue.empty();
    }

    ThreadPool(size_t threadnum = DefaultNums)
        : _threadnum(threadnum)
    {
        // 提前开好空间
        _threads.reserve(threadnum);
    }

    ThreadPool(const ThreadPool&) = delete;
    const ThreadPool& operator=(const ThreadPool&) = delete;

    void HandlerTask()
    {
        while(true)
        {
            // 上锁，同一时间只能一个线程访问资源
            std::unique_lock<std::mutex> lck(_mtx);

            // std::cout << "I am Blocked !!!" << std::endl;
            // // 当没有任务时阻塞,直至有任务

            _cond.wait(lck, [this](){return !IsEmpty();});
            // while(IsEmpty())//为空的话去等待
            //     pthread_cond_wait(&cond, &mtx);


            func_t func = _task_queue.front();
            _task_queue.pop();
            
            // pthread_mutex_unlock(&mtx);

            // 处理任务
            func();
        }
    }

public:
    void Init()
    {   
        for(int i = 0; i < _threadnum; i++)
        {
            std::string name = "thread_" + std::to_string(i + 1);
            func_t func = std::bind(&ThreadPool::HandlerTask, this);
            _threads.emplace_back(std::make_unique<Thread>(name, func));
        }

        std::cout << "Init all threads!!!" << std::endl;
    }

    void Start()
    {
        for(auto& thread : _threads)
        {
            thread->Start();
        }
        std::cout << "Start all threads!!!" << std::endl;
    }

    void Push(func_t& in)
    {
        // 上锁，同一时间只能一个线程访问资源
        std::unique_lock<std::mutex> lck(_mtx);

        // pthread_mutex_lock(&mtx);
        
        _task_queue.push(in);
        
        // pthread_mutex_unlock(&mtx);

        // std::cout << "WakeUp!!!" << std::endl;
        // pthread_cond_broadcast(&cond);
        _cond.notify_all();
    }

    static ThreadPool* GetInstance()
    {
        if(_tp == nullptr)
        {
            std::lock_guard<std::mutex> lck(_Sigmtx);
            if(_tp == nullptr)
            {
                _tp = new ThreadPool;
            }
        }

        return _tp;
    }


private:
    size_t _threadnum;
    std::vector<std::unique_ptr<Thread>> _threads;
    std::queue<func_t> _task_queue;
    
    std::mutex _mtx;
    std::condition_variable _cond;

    static ThreadPool* _tp;
    static std::mutex _Sigmtx;
};

ThreadPool* ThreadPool::_tp = nullptr;
std::mutex ThreadPool::_Sigmtx;
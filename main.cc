#include "Thread.hpp"

int tickets = 100;

// 定义一把全局的锁
// pthread_mutex_t gmutex = PTHREAD_MUTEX_INITIALIZER; 

void route(ThreadData *td)
{
    while (true)
    {
        // 上锁，避免资源竞争
        // pthread_mutex_lock(&gmutex);
        pthread_mutex_lock(td->_lock);
        if (tickets > 0)
        {
            usleep(1000);
            std::cout << td->_name << " get a tickets, there left " << tickets << std::endl;
            tickets--;
            pthread_mutex_unlock(td->_lock);
        }
        else 
        {
            pthread_mutex_unlock(td->_lock);
            break; 
        }
    }
}

const int gnum = 5;

int main()
{

    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, nullptr);

    std::vector<Thread> threads;
    for (int i = 0; i < gnum; i++)
    {
        std::string name = "thread_" + std::to_string(i + 1);
        ThreadData *td = new ThreadData(name, &mutex);

        threads.emplace_back(name, route, td);
    }

    // 等待所有线程就位
    sleep(2);
    
    // 启动
    for (auto &thread : threads)
    {
        thread.Start();
    }

    // 回收
    for (auto &thread : threads)
    {
        thread.Join();
    }

    pthread_mutex_destroy(&mutex);

    return 0;
}
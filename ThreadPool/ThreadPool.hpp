#include <thread>
#include <atomic>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>

#define MAXTHREADNUM 4
using Task = std::function<void()>; 

class ThreadPool
{
private:
    // 线程入口函数
    void threadEntrance() {
        while (true) {
            std::unique_lock<std::mutex> lck(_mtx);
            // 执行条件
            _cond.wait(lck, [this]{ return !_queue.empty() || !_running; });
            
            // 避免泄露任务
            if (!_running && _queue.empty()) {
                return;
            }

            // 取出任务执行
            Task tsk = std::move(_queue.front());
            _queue.pop();
            lck.unlock();
            tsk();
        }
    }

public:
    explicit ThreadPool(int numThreads = MAXTHREADNUM)
        : _threadNum(numThreads)
        , _threads(_threadNum)
        , _running(false)
    {}

    ~ThreadPool() {
        stop();
        // 回收线程
        for (std::thread &th : _threads) {
            th.join();
        }
    }

    void Start() {
        // 开始运行
        _running = true;
        for (int i = 0; i < _threadNum; i++) {
            // 每一个线程执行入口函数
            _threads[i] = std::thread(&ThreadPool::threadEntrance, this);
        }   
    }

    void stop() {
        // 停止执行
        _running = false;
        _cond.notify_all();
    }

    // 添加任务
    void addTask(Task task) {
        if (!_running) {
            // 不合理的请求，抛出异常处理
            throw std::runtime_error("Its not start.");
        }

        // 添加任务
        std::unique_lock<std::mutex> lck(_mtx);
        _queue.push(std::move(task));
        lck.unlock();
        _cond.notify_all();
    }

private:
    int _threadNum; // 线程数量
    std::vector<std::thread> _threads; // 管理线程
    std::mutex _mtx; 
    std::condition_variable _cond;
    std::queue<Task> _queue;  // 任务队列
    std::atomic<bool> _running; // 运行标识符
};
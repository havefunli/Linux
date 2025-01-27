#include <thread>
#include <future>
#include <atomic>
#include <vector>
#include <queue>
#include <mutex>
#include <memory>
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
            if (th.joinable()) {
                th.join();
            }
        }
    }

    void Start() {
        // 错误的逻辑
        if (_running = true) {
            return;
        }

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

    /*
    使用模板：可以传递任意类型的函数类型
    可变参数：传递任意数量的参数
    */ 
    template<class Func, class... Args>
    auto addTask(Func &&f, Args&&... args)-> std::future<decltype(f(args...))> {
        if (!_running) {
            // 不合理的请求，抛出异常处理
            throw std::runtime_error("ThreadPool is not running.");
        }

        // 推断返回类型
        using returnType = decltype(f(args...));
        
        // 使用 std::packaged_task 来封装任务
        // 再使用 shared_ptr 来指向该任务，以便后续传参
        auto task = std::make_shared<std::packaged_task<returnType()>>(std::bind(std::forward<Func>(f), std::forward<Args>(args)...));
        std::future<returnType> fut = task->get_future();    

        // 添加任务到队列
        std::unique_lock<std::mutex> lck(_mtx);
        _queue.emplace([task](){ (*task)(); });
        lck.unlock();

        // 通知一个等待的线程
        _cond.notify_all();

        return fut;
    }

private:
    int _threadNum; // 线程数量
    std::vector<std::thread> _threads; // 管理线程
    std::mutex _mtx; 
    std::condition_variable _cond;
    std::queue<Task> _queue;  // 任务队列
    std::atomic<bool> _running; // 运行标识符
};
#include <thread>
#include <string>
#include <iostream>
#include <functional>

// 该线程经过包装
// 关键位置打印信息便于观察
// 传递给线程的任务需要经过处理，一律无参
using func_t = std::function<void()>;

class Thread
{
public:
    Thread(std::string name, func_t func)
        : _name(name)
        , _func(func)
    {
        std::cout << _name << " successful load!" << std::endl;
    }

    void Start()
    {
        _td = std::thread(_func);
    }

    ~Thread()
    {
        _td.join();
    }

private:
    std::thread _td;
    std::string _name;
    func_t _func;
};



#include <queue>
#include <future>
#include <chrono>
#include <thread>
#include <functional>
#include <iostream>

int add(int x, int y) {
    std::this_thread::sleep_for(std::chrono::seconds(3));
    return x + y;
}

// int main() {
//     // 将一个可调用对象封装为 packaged_task
//     std::packaged_task<int(int, int)> tsk(add);
//     // 获取与之相对应的 future
//     std::future res = tsk.get_future();
//     // 执行该任务
//     int x = 1, y = 2;
//     std::thread th(tsk, x, y);

//     printf("I am waiting!\n");
//     int result = res.get();
//     printf("The result is %d\n", result);

//     th.join();

//     return 0;
// }

int main() {
    auto func = std::bind(add, 1, 2);
    std::cout << func() << std::endl;
    return 0;
}
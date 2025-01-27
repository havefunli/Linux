#include "ThreadPool_Plus.hpp"
#include <ctime>
#include <iostream>

int add(int l, int r) {
    int ans = 0;
    for (; l < r; l++) {
        ans += l;
    }
}

void test1() {
    ThreadPool pool;
    pool.Start();
    for (int i = 1; i < 10000; i++) {
        std::future<int> fut = pool.addTask(add, 0, i);
        std::cout << fut.get() << " ";
    }
}

int main() {
    clock_t start, end;
    
    start = clock();
    test1();
    end = clock();
    std::cout << "thread pool takes " << (double)(end - start) / CLOCKS_PER_SEC << "s." << std::endl;

    start = clock();
    for (int i = 1; i < 10000; i++) {
        add(0, i);
    }
    end = clock();
    std::cout << "thread takes " << (double)(end - start) / CLOCKS_PER_SEC << "s." << std::endl;

    return 0;
}
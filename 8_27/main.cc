#include "RingQueue.hpp"

void *Productor(void *arg)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(arg);
    while(true)
    {
        rq->push(rand() % 10 + 1);
        sleep(1);
    }
}

void *Consumer(void *arg)
{
    RingQueue<int> *rq = static_cast<RingQueue<int> *>(arg);
    while(true)
    {
        int val = 0;
        rq->pop(val);
        std::cout << "I got a val : " << val << std::endl;
    }
}


int main()
{
    srand(time(nullptr) ^ 777);
    RingQueue<int> *ptr = new RingQueue<int>(5);
    
    pthread_t p1, p2, c1, c2;
    pthread_create(&p1, nullptr, Productor, ptr);
    pthread_create(&p2, nullptr, Productor, ptr);
    pthread_create(&c1, nullptr, Consumer, ptr);
    pthread_create(&c2, nullptr, Consumer, ptr);


    pthread_join(p1, nullptr);
    pthread_join(p2, nullptr);
    pthread_join(c1, nullptr);
    pthread_join(c2, nullptr);

    delete ptr;

    return 0;
}
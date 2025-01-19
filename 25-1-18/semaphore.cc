#include <iostream>
#include <semaphore.h>
#include <pthread.h>

int cnt = 0;
sem_t src;
sem_t space;

void P(sem_t *sem) {
    sem_wait(sem);
}

void V(sem_t *sem) {
    sem_post(sem);
}

void *Productor(void *) {
    while (true) {
        P(&space);
        std::cout << "产生一个数据" << std::endl;
        V(&src);
    }

    return nullptr;
}

void *Consumer(void *) {
    while (true) {
        P(&src);
        std::cout << "消费一个数据" << std::endl;
        V(&space);
    }

    return nullptr;
}


int main () {
    sem_init(&src, 0, 0);
    sem_init(&space, 0, 1);

    pthread_t p1, p2;
    pthread_create(&p1, nullptr, Productor, nullptr);
    pthread_create(&p2, nullptr, Consumer, nullptr);

    pthread_join(p1, nullptr);
    pthread_join(p2, nullptr);

    sem_destroy(&src);
    sem_destroy(&space);
    return 0;
}
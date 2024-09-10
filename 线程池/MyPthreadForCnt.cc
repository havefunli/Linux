#include <iostream>
#include <pthread.h>
#include <vector>
#include <string>
#include <unistd.h>

// int Tickets = 10000;
// pthread_mutex_t gmutex = PTHREAD_MUTEX_INITIALIZER;
// pthread_cond_t gcond = PTHREAD_COND_INITIALIZER;

// class MyThread
// {
// public:
//     MyThread(const std::string &name)
//         : _name(name), _cnt(0) {}

//     void Excute()
//     {
//         pthread_create(&_tid, nullptr, func_t, this);
//     }

//     void Join()
//     {
//         pthread_join(_tid, nullptr);
//     }

//     std::string getName() const
//     {
//         return _name;
//     }

//     int getCnt() const
//     {
//         return _cnt;
//     }

//     void CountIncrease()
//     {
//         _cnt++;
//     }

//     ~MyThread()
//     {
//         // Destructor doesn't need to join threads if explicitly joined
//     }

// private:
//     static void *func_t(void *arg)
//     {
//         MyThread *Ptr = static_cast<MyThread *>(arg);
//         while (true)
//         {
//             pthread_mutex_lock(&gmutex);
//             if (Tickets > 0)
//             {
//                 pthread_cond_wait(&gcond, &gmutex);
//                 Tickets--;
//                 std::cout << Ptr->getName() << " got a ticket, there are " << Tickets << " left." << std::endl;
//                 pthread_mutex_unlock(&gmutex);
//                 Ptr->CountIncrease();
//             }
//             else
//             {
//                 pthread_mutex_unlock(&gmutex);
//                 break;
//             }
//         }
//         return nullptr;
//     }

//     std::string _name;
//     pthread_t _tid;
//     int _cnt;
// };

// const int NUMOFTHREAD = 5;

// int main()
// {
//     std::vector<MyThread> vec;
//     vec.reserve(NUMOFTHREAD);

//     // Create threads
//     for (int i = 0; i < NUMOFTHREAD; i++)
//     {
//         std::string name = "Thread_" + std::to_string(i + 1);
//         vec.emplace_back(name);
//     }

//     for (int i = 0; i < NUMOFTHREAD; i++)
//     {
//         vec[i].Excute();
//     }

//     while(true)
//     {
//         pthread_cond_signal(&gcond);
//     }

//     // Wait for all threads to finish
//     for (int i = 0; i < NUMOFTHREAD; i++)
//     {
//         vec[i].Join();
//         std::cout << vec[i].getName() << " did it " << vec[i].getCnt() << " times." << std::endl;
//     }

//     return 0;
// }

pthread_mutex_t gmutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t gcond = PTHREAD_COND_INITIALIZER;

void *Print(void *arg)
{
    std::string name = static_cast<const char *>(arg);
    while (true)
    {
        pthread_mutex_lock(&gmutex);

        pthread_cond_wait(&gcond, &gmutex);
        std::cout << "I am " << name << std::endl;

        pthread_mutex_unlock(&gmutex);
    }
}

const int num = 5;

int main()
{
    pthread_t threads[num];
    for (int i = 0; i < num; i++)
    {
        char *name = new char[64];
        snprintf(name, 64, "thread_%d", i + 1);
        pthread_create(threads + i, nullptr, Print, (void*)name);
    }

    while (true)
    {
        pthread_cond_signal(&gcond);
        sleep(1);
    }

    for (int i = 0; i < num; i++)
    {
        pthread_join(threads[i], nullptr);
    }

    return 0;
}

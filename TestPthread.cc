#include <cerrno>
#include <unistd.h>
#include <pthread.h>

#include <iostream>  
#include <pthread.h>  
#include <unistd.h> // 包含 sleep 函数的定义  
  
void* thread_Func(void* arg)   
{    
    int cnt = 0;
    while(true)
    {
        std::cout << "I am doing my job, pthread_id is 1!" << std::endl;
        if(cnt++ == 5)
        {
            break;
        }

        sleep(1);
    }

    return nullptr;
}  
  
int main()  
{  
    int id = 0;  
    pthread_t tid;  
  
    // 创建线程  
    if (pthread_create(&tid, NULL, thread_Func, &id) != 0) 
    {  
        perror("pthread_create");  
        exit(1);  
    }  
  
    // 线程分离
    if(pthread_detach(tid) != 0)
    {
        perror("pthread_create");  
        exit(1); 
    }

    int cnt = 0;
    while(true)
    {
        std::cout << "I am doing my job, pthread_main!" << std::endl;
        if(cnt++ == 5)
        {
            break;
        }

        sleep(1);
    }
  
    return 0;  
}
#include <iostream>

#include <unistd.h>
#include <sys/types.h>
#include <signal.h>


int main()
{
    int cnt = 0;
    while(true)
    {
        std::cout << "I am Running, my pid is " << getpid() << std::endl;
        sleep(1);

        cnt++;
        if(cnt == 5)
        {
            kill(getpid(), 9);
        }
    }
    return 0;
}

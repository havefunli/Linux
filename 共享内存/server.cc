#include "MyShm.hpp"

int main()
{
    Shm shm(_WorkPath, _Proj_id, CREATER);
    char* shmaddr = (char*)shm.GetShmAddr();
    
    while(true)
    {
        std::cout << "The message is: " << shmaddr << std::endl;
        sleep(2);

        if(shmaddr[strlen(shmaddr) - 1] > 'Z') { return 0; };
    }

    return 0;
}
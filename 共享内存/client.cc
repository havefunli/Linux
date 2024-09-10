#include "MyShm.hpp"

int main()
{
    Shm shm(_WorkPath, _Proj_id, CREATER);
    char* shmaddr = (char*)shm.GetShmAddr();
    
    char ch = 'A';
    while(ch <= 'Z')
    {
        shmaddr[ch - 'A'] = ch;
        std::cout << "Already sent to server!!!" << std::endl;
        ch++;
        sleep(2);
    }

    return 0;
}
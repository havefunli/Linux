#include <iostream>
#include <unistd.h>

int main()
{
    char buf[1024];

    int n = read(0, buf, sizeof(buf) - 1);
    if (n > 0)
    {
        buf[n] = '\0';
    }   
    std::cout << buf << std::endl;

    return 0;
}

#include "NamedPipe.hpp"

int main()
{
    NamedPipe serve(Creater);

    if (serve.OpenForRead())
    {
        std::cout << "I am ready, serve!" << std::endl;
        while (true)
        {
            std::string msg;
            int n = serve.Receive(msg);

            if (n > 0)
            {
                std::cout << "Client say: " << msg << std::endl;
            }
            else if (n == 0)
            {
                std::cout << "Client quit, Server Too!" << std::endl;
                break;
            }
            else
            {
                std::cout << "Error" << std::endl;
                break;
            }
        }
    }

    return 0;
}
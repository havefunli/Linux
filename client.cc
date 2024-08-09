#include "NamedPipe.hpp"

int main()
{
    NamedPipe client(USER);

    if(client.OpenForWrite())
    {
        std::cout << "I am ready, client!" << std::endl;
        while(true)
        {
            std::cout << "Please send your messge: ";
            std::string msg;
            std::getline(std::cin, msg);

            client.send(msg);
        }
    }

    return 0;
}

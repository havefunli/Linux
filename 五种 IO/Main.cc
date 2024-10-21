#include "EpollServer.hpp"


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: ./server port" << std::endl;
        return -1;
    }

    uint16_t port = std::stoi(argv[1]);
    std::unique_ptr<EpollServer> svrptr = std::make_unique<EpollServer>(port);
    svrptr->Loop();
    
    return 0;
}

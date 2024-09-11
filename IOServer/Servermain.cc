#include "UdpServer.hpp"


int main()
{
    UdpServer server(8888);
    server.Init();
    server.Start();

    return 0;
}
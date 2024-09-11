#include "UdpClient.hpp"


int main()
{
    UdpClient client("127.0.0.1", 8888);
    client.Init();
    client.Start();

    return 0;
}
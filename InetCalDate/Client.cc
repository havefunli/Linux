#include "Socket.hpp"


int main()
{
    TcpSocket client;
    client.BuildTcpClient("127.0.0.1", 8888);

    std::string msg = "ABCD";
    client.Send(msg);

    return 0;
}
#include "Socket.hpp"


int main()
{
    TcpSocket service;
    service.BuildTcpListen(8888);

    SockAddHelper clientaddr;
    SockPtr Ptr = service.AcceptConnect(clientaddr);
    
    std::string out;
    Ptr->Recv(out);
    std::cout << out << std::endl;

    return 0;
}
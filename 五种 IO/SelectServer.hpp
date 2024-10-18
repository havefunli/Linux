#include "ToolForSockAddr_in.hpp"
#include "Socket.hpp"


class SelectServer
{
public:
    SelectServer(uint16_t port)
        : _port(port)
        , _listensock(std::make_unique<TcpSocket>())
    {
        _listensock->BuildTcpListen(_port);
        std::cout << "Successful init SelectServer..." << std::endl;
    }

    void Loop()
    {
        while (true)
        {
            SockAddHelper client;
            _listensock->AcceptConnect(client);
        }
    }

private:
    uint16_t _port;
    std::unique_ptr<Socket> _listensock;
};
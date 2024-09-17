#include "TcpServer.hpp"
#include "HTTP.hpp"

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        std::cerr << "Usage: ./server port" << std::endl;
        return(1);
    }

    uint16_t port = std::stoi(argv[1]);

    HTTP httpserver;

    service_t func = std::bind(&HTTP::HttpServer, httpserver, std::placeholders::_1);
    std::unique_ptr<TcpServer> tcpserver = std::make_unique<TcpServer>(func, port);

    tcpserver->Loop();

    return 0;
}
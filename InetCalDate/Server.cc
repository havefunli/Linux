#include "TcpServer.hpp"
#include "Service.hpp"
#include "InetDateCal.hpp"

int main()
{
    InetCalDate cal;
    Service service(std::bind(&InetCalDate::Calculate, cal, std::placeholders::_1));

    service_t func = std::bind(&Service::Execute, &service, std::placeholders::_1, std::placeholders::_2);
    std::unique_ptr<TcpServer> tcpserver = std::make_unique<TcpServer>(func, 8888);

    tcpserver->Loop();

    return 0;
}
#include "TcpClient.hpp"
#include "Client.hpp"
#include "Protocol.hpp"

int main()
{
    Client client;
    auto func = std::bind(&Client::Execute, client, std::placeholders::_1);
    std::unique_ptr<TcpClient> tcpclient = std::make_unique<TcpClient>(func, 8888);

    tcpclient->Loop();

    return 0;

    // Date d1(1, 1, 1);
    // Request Reqst(d1, d1, '-');

    // std::string req = Reqst.Serialize();
    // std::cout << req << std::endl;

    // Encode(req);
    // std::cout << "Successful encode..." << std::endl;
    // std::cout << req << std::endl;

    // std::string str = Decode(req);
    // std::cout << "Successful decode..." << std::endl;
    // std::cout << str << std::endl;

    // Reqst.Deserialize(str);
    // std::cout << "Successful deserialize..." << std::endl;
    // std::cout << str << std::endl;

    // std::cout << Reqst.GetLeft() << std::endl;
}
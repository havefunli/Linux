// 客户端需要指定相连的服务端的 IP 和 端口

#include "UdpClient.hpp"

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " server_ip server_port" << std::endl;
        exit(0);
    }

    std::string server_ip = argv[1];
    uint16_t server_port = std::stoi(argv[2]);

    UdpClient client(server_ip, server_port);
    client.Init();
    client.Start();

    return 0;
}
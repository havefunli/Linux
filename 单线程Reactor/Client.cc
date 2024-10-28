#include "Socket.hpp"
#include "InetDateCal/Protocol.hpp"
#include <memory>

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: ./client Ip Port" << std::endl;
        exit(-1);
    }

    string Ip = argv[1];
    uint16_t Port = std::stoi(argv[2]);

    unique_ptr<Socket> client_sock = std::make_unique<TcpSocket>();
    client_sock->BuildTcpClient(Ip, Port);

    while (true)
    {
        // 输入信息
        Date left;
        std::cout << "请输入日期 1 : ";
        std::cin >> left;
        Date right;
        std::cout << "请输入日期 2 : ";
        std::cin >> right;

        std::unique_ptr<Request> ReqPtr = std::make_unique<Request>(left, right);
        // 序列化
        std::string Req = ReqPtr->Serialize();
        std::cout << Req << std::endl;
        // 添加报头
        Encode(Req);
        std::cout << Req << std::endl;

        // 发送到服务端
        ssize_t n = client_sock->Send(Req);
        if (n > 0)
            std::cout << "Successful Send..." << std::endl;
        else
        {
            perror("send:");
            continue;
        }

        // 接收信息
        std::string packagestream;
        bool ret = client_sock->Recv(packagestream);
        if (ret)
            std::cout << "Successful recv..." << std::endl;
        else
            continue;
        std::cout << packagestream << std::endl;

        // 报文解析
        std::string package = Decode(packagestream);
        if (package.empty())
        {
            std::cerr << "Error Decode..." << std::endl;
            continue;
        }
        std::cout << "Successful decode..." << std::endl;
        std::cout << package << std::endl;

        // 反序列化
        std::shared_ptr<Response> ResPtr = std::make_shared<Response>();
        ResPtr->Deserialize(package);
        std::cout << "Successful deserialize..." << std::endl;

        // 输出结果
        std::cout << "最终结果是：" << std::endl;
        std::cout << ResPtr->GetResult() << std::endl;
    }

    return 0;
}
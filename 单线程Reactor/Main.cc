#include "Listener.hpp"
#include "NormalConnection.hpp"
#include "Reactor.hpp"
#include "PackageParse.hpp"


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage ./main Port" << std::endl;
        exit(1);
    }
    uint16_t port = std::stoi(argv[1]);

    Listener Server(port);
    SockAddHelper local("0.0.0.0", port);

    PackageParse parser;
    NormalConn Client(std::bind(&PackageParse::Execute, &parser, std::placeholders::_1));
    
    std::unique_ptr<Reactor> R = std::make_unique<Reactor>();
    // 监听套接字读逻辑
    R->SetListenHanlder(std::bind(&Listener::Accepter, &Server, std::placeholders::_1));
    // 普通套接字读写异常逻辑
    R->SetNormalHanlder(std::bind(&NormalConn::HandleRecv, &Client, std::placeholders::_1), 
                        std::bind(&NormalConn::HandleSend, &Client, std::placeholders::_1),
                        std::bind(&NormalConn::HandleExcept, &Client, std::placeholders::_1));

    R->AddConnection(Server.SockFd(), 
                    EPOLLIN,
                    
                    ConnType::ListenConnection,
                    local);
    R->Dispatch();

    return 0;
}
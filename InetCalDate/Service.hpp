#pragma once

#include "ToolForSockAddr_in.hpp"
#include "Socket.hpp"
#include "Protocol.hpp"

#include <functional>
#include <memory>

using process_t = std::function<std::shared_ptr<Response>(std::shared_ptr<Request>)>;

class Service
{
public:
    Service() {}
    Service(process_t task)
        : _task(task)
    {}

    void Execute(SockPtr& sock, SockAddHelper& addr)
    {
        std::cout << "Waiting for data..." << std::endl;
        std::string packagestream;
        while(true)
        {
            // 接收消息
            bool ret = sock->Recv(packagestream);
            if(ret) std::cout << "Successful recv..." << std::endl;
            else break;
            std::cout << packagestream << std::endl;

            // 报文解析
            std::string package = Decode(packagestream);
            if(package.empty()) continue;
            std::cout << "Successful decode..." << std::endl;
            std::cout << package << std::endl;
            
            // 反序列化
            std::shared_ptr<Request> ReqPtr = std::make_shared<Request>();
            ReqPtr->Deserialize(package);
            std::cout << "Successful deserialize..." << std::endl;

            // 内容处理
            std::shared_ptr<Response> ResPtr = _task(ReqPtr);
            std::cout << "Successful handletask..." << std::endl;
            std::cout << ResPtr->GetResult() << std::endl;

            // 序列化
            std::string ResultReq = ResPtr->Serialize();
            std::cout << "Successful Serialize..." << std::endl;
            std::cout << ResultReq << std::endl;

            // 添加报头
            Encode(ResultReq);
            std::cout << "Successful encode..." << std::endl;
            std::cout << ResultReq << std::endl;

            // 发送到客户端
            int n = sock->Send(ResultReq);
            if(n < 0) 
            {
                perror("send");
                continue;
            }
            else
            {
                std::cout << "Successful send..." << std::endl;
            }
        }

        std::cout << "Quit..." << std::endl;
        close(sock->GetFd()); 
    }


    ~Service(){}

private:
    process_t _task;
};
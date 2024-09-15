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

            // 报文解析
            std::string package = Decode(packagestream);
            if(package.empty()) continue;
            std::cout << package << std::endl;
            std::cout << "Successful decode..." << std::endl;
            
            // 反序列化
            std::shared_ptr<Request> ReqPtr;
            std::cout << "Begin deserialize..." << std::endl;
            ReqPtr->Deserialize(package);
            std::cout << "Successful deserialize..." << std::endl;

            // 内容处理
            std::shared_ptr<Response> ResPtr = _task(ReqPtr);

            // 序列化
            std::string ResultReq = ReqPtr->Serialize();

            // 添加报头
            Encode(ResultReq);

            // 发送到客户端
            sock->Send(ResultReq);
        }
    }


    ~Service(){}

private:
    process_t _task;
};
#pragma once

#include "ToolForSockAddr_in.hpp"
#include "ThreadPool.hpp"

#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <functional>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>



class Forward
{

public:
    bool IsOnline(const TFSA_in& id)
    {
        auto iter = std::find(_group.begin(), _group.end(), id);
        if(iter == _group.end()) return false;
        else return true;
    }

    void AddUsr(const TFSA_in& id)
    {
        if(!IsOnline(id)) _group.push_back(id);
    }

    void Offline(const TFSA_in& id)
    {
        auto iter = std::find(_group.begin(), _group.end(), id);
        _group.erase(iter);
    }
    
    void forward_msg(int sockfd, const TFSA_in& id, const std::string& msg)
    {
        // 若用户不在线，添加到用户组
        AddUsr(id); 

        // 特殊信息就离线
        if(msg == "Quit") Offline(id);

        // 多线程执行分发任务
        
    }


private:
    std::vector<TFSA_in> _group; // 在线用户组
};
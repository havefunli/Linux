#pragma once

#include "ToolForSockAddr_in.hpp"
#include "Socket.hpp"

#include <vector>
#include <functional>
#include <memory>

const std::string SEP = "\r\n";
const std::string BlankLine = "\r\n";

class HTTPRequest
{
private:
    std::string GetLine(std::string& reqstr)
    {
        if(reqstr.empty()) return std::string();
        
        auto pos = reqstr.find(SEP);
        if(pos == std::string::npos) return std::string();

        std::string line = reqstr.substr(0, pos);
        reqstr.erase(0, line.size() + SEP.size());
        
        return line;
    }

public:
    HTTPRequest() {}


    void Deserialize(std::string& reqstr)
    {
        // 读取请求行
        _req_line = GetLine(reqstr);
        std::cout << _req_line << std::endl;

        // 读取报头
        std::string header;
        while(1)
        {
            header = GetLine(reqstr);
            std::cout << header << std::endl;
            if(header == BlankLine || header == "") break;
            _req_headers.push_back(std::move(header));
        }
        // 读取正文
        _content = reqstr;
        std::cout << _content << std::endl;
    }

    // For Debug
    void Print()
    {
        std::cout << "REQLine: ";
        std::cout << _req_line << std::endl;

        for(auto& header : _req_headers)
        {
            std::cout << "REQHeaders: ";
            std::cout << header << std::endl;
        }

        std::cout << "Content: ";
        std::cout << _content << std::endl;
        std::cout << _content.size() << std::endl;
    }


private:
    std::string _req_line;
    std::vector<std::string> _req_headers;
    std::string _content;
};


class HTTP
{
public:
    HTTP() {}

    void HttpServer(std::string reqstr)
    {
        HTTPRequest request;
        std::cout << "Begin to deserialize..." << std::endl;
        request.Deserialize(reqstr);
        std::cout << "Done..." << std::endl;

        request.Print();
    }
};
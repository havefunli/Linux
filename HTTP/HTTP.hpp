#pragma once

#include "ToolForSockAddr_in.hpp"
#include "Socket.hpp"


#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include <functional>
#include <unordered_map>

const std::string SEP = "\r\n";
const std::string BlankLine = "\r\n";
const std::string HeaderSep = ": ";
const std::string Space = " ";
const std::string SuffixSep = ".";
const std::string DefaultSuffix = ".default";
const std::string DefaultPathFor_404 = "wwwroot/404.html";
const std::string HomePage = "index.html";
const std::string PrefixPath = "wwwroot";
const std::string DefaultHttpVersion = "HTTP/1.0";

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
        
        return line.empty() ? BlankLine : line;
    }

    void ParseReqLine(std::string& line)
    {
        std::stringstream ss(line);
        ss >> _method >> _url >> _httpversion;
    }

    void ParseReqHeaders(std::vector<std::string>& headers)
    {
        for(auto& head : headers)
        {
            auto pos = head.find(HeaderSep);
            if(pos == std::string::npos) continue;

            std::string key = head.substr(0, pos);
            std::string val = head.substr(pos + HeaderSep.size());
            if(val.empty()) continue;

            _req_headers[key] = val;
        }
    }

    void InitPath()
    {
        _path += _url;
        if(_url == "/")
        {
            _path += HomePage;
        }
    }

public:
    // 默认的资源路径
    HTTPRequest(std::string Path = PrefixPath) 
            : _path(PrefixPath)
        {}


    void Deserialize(std::string& reqstr)
    {
        // 读取请求行, 并分解为 请求方法 | URL | HTTP版本
        std::string ReqLine = GetLine(reqstr);
        ParseReqLine(ReqLine);
        
        // 正确初始化需要请求资源的路劲
        InitPath();

        // 读取报头, 并转化为键值对方式
        std::string header;
        std::vector<std::string> _req_headers;
        while(1)
        {
            header = GetLine(reqstr);
            if(header == BlankLine) break;
            _req_headers.push_back(std::move(header));
        }
        ParseReqHeaders(_req_headers);

        // 读取正文
        _content = reqstr;
    }

    std::string GetPath()
    {
        return _path;
    }

    // For Debug
    void Print()
    {
        std::cout << "METHOD: " << _method << std::endl;
        std::cout << "URL: " << _url << std::endl; 
        std::cout << "HTTP: " << _method << std::endl; 
        
        for(auto& head : _req_headers)
        {
            std::cout << "HEAD: " << head.first << ": " << head.second << std::endl;
        }

        std::cout << "CONTENT: " << _content << std::endl; 
    }


private: 
    std::string _method;
    std::string _url;
    std::string _path;
    std::string _httpversion;
    std::unordered_map<std::string, std::string> _req_headers;
    std::string _content;
};

class HTTPResponse
{
public:
    HTTPResponse(std::string Version = DefaultHttpVersion)
        : _httpversion(Version)
    {}

    // 设置版本
    void SetVersion(const std::string& Version)
    {
        _httpversion = Version;
    }

    // 设置状态码和描述
    void SetStatus(int Code, const std::string& desc)
    {
        _status_code = Code;
        _desc = desc;
    }

    // 设置报头
    void AddHeader(const std::string& Key, const std::string& Val)
    {
        _req_headers[Key] = Val;
    }

    // 写入内容
    void SetContent(const std::string& content)
    {
        _content = content;
    }

    std::string Serialize()
    {
        // 构建请求行
        std::string ReqLine = _httpversion + Space 
                            + std::to_string(_status_code) + Space 
                            + _desc + SEP;

        // 构建报头
        std::string Headers;
        for(auto& header : _req_headers)
        {
            Headers += (header.first + HeaderSep + header.second + SEP);
        }

        return ReqLine + Headers + BlankLine + _content;
    }

private:
    std::string _httpversion;
    int _status_code;
    std::string _desc;
    std::unordered_map<std::string, std::string> _req_headers;
    std::string _content;
};



class HTTP
{
private:
    void SetSuffix(std::string path)
    {
        auto pos = path.rfind(SuffixSep);
        if(pos == std::string::npos) 
        {
            _suffix = DefaultSuffix;
        }
        else
        {
            _suffix = path.substr(pos);
        }
    }

    std::string GetFileContent(std::string path)
    {
        std::ifstream in(path, std::ios::binary);
        if(!in.is_open()) return std::string("");

        in.seekg(0, in.end);
        int size = in.tellg();
        in.seekg(0, in.beg);

        std::string content(size, 0);
        in.read((char*)content.c_str(), size); 
        in.close();

        SetSuffix(path);

        return content; 
    }

public:
    HTTP() 
    {
        _suffixs_table.insert(std::make_pair(".html", "text/html"));
        _suffixs_table.insert(std::make_pair(".jpg", "image/jpeg"));
        _suffixs_table.insert(std::make_pair(".png", "image/png"));
        _suffixs_table.insert(std::make_pair(".default", "text/html"));
    }

    void HttpServer(SockPtr sock)
    {
        // 获取请求
        std::string reqstr;
        sock->Recv(reqstr);
        std::cout << "Successful Recv..." << std::endl;
        std::cout << reqstr<< std::endl;

        // 序列化
        HTTPRequest request;
        request.Deserialize(reqstr);
        std::cout << "Successful deserialize..." << std::endl;

        // 获取内容
        HTTPResponse response;
        std::string content = GetFileContent(request.GetPath());
        std::cout << "Successful get content..." << std::endl;

        // 处理非法请求
        if(content.empty())
        {   
            response.SetStatus(404, "Not Fund");
            content = GetFileContent(DefaultPathFor_404);
        }
        else
        {
            response.SetStatus(200, "OK");
        }
        
        // 初始化响应字段
        response.SetContent(content);
        response.AddHeader("Content-Length", std::to_string(content.size()));
        response.AddHeader("Content-Type", _suffixs_table[_suffix]);

        // 序列化
        std::string Req = response.Serialize();
        std::cout << "Successful serialize..." << std::endl;
        std::cout << Req << std::endl;

        // 发送消息
        int n = sock->Send(Req);
        if(n < 0) 
        {
            perror("send:");
        }
        else
        {
            std::cout << "Successful send..." << std::endl;
            std::cout << Req << std::endl;
        }
    }

private:
    std::string _suffix;
    std::unordered_map<std::string, std::string> _suffixs_table;
};
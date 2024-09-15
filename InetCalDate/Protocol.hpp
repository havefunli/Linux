#pragma once
#include <jsoncpp/json/json.h>
#include <string>
#include "Date.h"

const std::string sep = "\r\n";

std::string Decode(std::string& packagestream)
{
    auto pos = packagestream.find(sep);
    if(pos == std::string::npos) return std::string();

    std::string lenstr = packagestream.substr(0, pos);
    int len = std::stoi(lenstr);

    int totalsize = lenstr.size() + 2 * sep.size() + len;
    if(packagestream.size() < totalsize) return std::string();

    std::string jsonstr = packagestream.substr(pos + sep.size(), len);
    packagestream.erase(0, totalsize);
    return jsonstr;
}

void Encode(std::string &jsonstr) 
{
    int len = jsonstr.size();
    jsonstr = std::to_string(len) + sep + jsonstr + sep;
}


class Request
{
public:
    Request() {}
    Request(const Date& left, const Date& right, char Oper = '-') 
        : _left(left)
        , _right(right)
        , _Oper(Oper)    
    {}

    std::string Serialize()
    {
        Json::Value root;
        root["Left"] = _left.toJosn();
        root["Right"] = _right.toJosn();
        root["Operator"] = _Oper;

        std::string Seq = Json::FastWriter().write(root);
        std::cout << "Successful serialize..." << std::endl;
        return Seq;
    }

    bool Deserialize(const std::string in)
    {
        Json::Value root;
        bool n = Json::Reader().parse(in, root);
        if(!n) std::cout << "error Reader()!!!" << std::endl;

        _left = Date(root["Left"]["Year"].asInt(),
                    root["Left"]["Month"].asInt(),
                    root["Left"]["Day"].asInt());

        std::cout << "Left: " << _left << std::endl;

        _right = Date(root["Right"]["Year"].asInt(),
                    root["Right"]["Month"].asInt(),
                    root["Right"]["Day"].asInt());

        std::cout << "Right: " << _right << std::endl;

        _Oper = root["Operator"].asInt();
        
        return n;
    }

    Date& GetLeft() 
    {
        return _left;
    }

    Date& GetRight()
    {
        return _right;
    }

    char GetOper()
    {
        return _Oper;
    }

    void Print()
    {
        std::cout << _left << " " << _Oper << " " << _right << std::endl;
    }

private:
    Date _left;
    Date _right;
    char _Oper;
};

class Response
{
public:
    Response() {}
    
    Response(int result) 
        : _result(result)
    {}

    std::string Serialize()
    {
        Json::Value root;
        root["Result"] = _result;

        std::string Req = Json::FastWriter().write(root);
        return Req;
    }

    bool Deserialize(const std::string &in)
    {
        Json::Value root;
        bool n = Json::Reader().parse(in, root);

        _result = root.asInt();

        return n;
    }

    int GetResult()
    {
        return _result;
    }

private:
    int _result;
};
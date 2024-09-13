#include "UdpServer.hpp"
#include "Translator.hpp"

int main()
{
    Translator dict;
    func_t func = std::bind(&Translator::translate, dict, std::placeholders::_1);

    UdpServer server(func, 8888);
    server.Init();
    server.Start();

    return 0;
}
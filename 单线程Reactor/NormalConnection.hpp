#include "Connection.hpp"


class NormalConn
{
public:
    NormalConn(handle_t Process)
        : _process(Process)
    {}

    void HandleRecv(Connection* conn)
    {
        while (true)
        {
            char Buf[DefaultSize];
            int n = recv(conn->SockFd(), Buf, DefaultSize - 1, 0);
            
            if (n > 0)
            {
                Buf[n - 1] = 0;
                conn->InBuff() += Buf;
            }
            else
            {
                if (errno == EWOULDBLOCK)
                {
                    std::cout << "Already read all data..." << std::endl;
                    break;
                }
                else if (errno == EINTR)
                {
                    continue;
                }
                else
                {
                    perror("recv");
                    return;
                }
            }
        }

        std::cout << "Buf-> " << conn->InBuff() << std::endl; 
        _process(conn);
    }

    void HandleSend(Connection* conn)
    {
        std::cout << "Send is ready..." << std::endl;
    }

    void HandleExcept(Connection* conn)
    {
        std::cout << "Except is ready..." << std::endl;
    }  

private:
    handle_t _process;
};
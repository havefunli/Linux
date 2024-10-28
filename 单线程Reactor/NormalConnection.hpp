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
                Buf[n] = 0;
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
        while (true)
        {  
            errno = 0;
            ssize_t n = send(conn->SockFd(), conn->OutBuff().c_str(), conn->OutBuff().size(), 0);
            if (n > 0)
            {
                conn->DiscardOutBuff(n);
                // 数据发送完了
                if (conn->OutBuff().empty()) 
                {
                    std::cout << "Send all data..." << std::endl;
                    break;
                }
            }
            else if (n == 0)
            {
                break;
            }
            else
            {
                // 发送条件不满足
                if (errno == EWOULDBLOCK) break;
                else if (errno == EINTR) continue;
                else
                {
                    conn->_handle_excepter(conn);
                    return;
                }
            }
        }

        if (!conn->OutBuff().empty())
        {
            conn->GetPtr()->EnableConnEvent(conn->SockFd(), EPOLLIN | EPOLLOUT);
        }
        else
        {
            conn->GetPtr()->EnableConnEvent(conn->SockFd(), EPOLLIN);
        }
    }

    void HandleExcept(Connection* conn)
    {
        std::cout << "Except is not ready..." << std::endl;
    }  

private:
    handle_t _process;
};
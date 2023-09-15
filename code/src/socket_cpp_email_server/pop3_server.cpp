#include "pop3_server.h"
#include "pop3_server_handler.h"
#include "TCPServer.h"
#include "Socket.h"
#include "SqliteDataBase.h"
auto logFunc2 = [](const std::string strRsp) {};
void StartPop3Server()
{
    std::string strPort = "2110";
    CTCPServer server(logFunc2, strPort);
    ASocket::Socket client;
    char buff[128] = {0};
    int recvLen = 0;
    while (true)
    {
        if (server.Listen(client, 1000))
        {
            auto dbPtr = std::make_shared<tiny_email::CSqliteDataBase>("sock_cpp_email.db");
            tiny_email::CPop3ServerHandler handler(dbPtr);

            while (true)
            {

                std::string strRsp = handler.GetResponse();
                if (!strRsp.empty())
                {
                    if (server.Send(client, strRsp))
                    {
                        std::cout << "S: " << strRsp << std::endl;
                    }
                    else
                    {
                        std::cout << "Send Failed" << std::endl;
                        break;
                    }
                }
                memset(buff, 128, 0);
                recvLen = -1;
                recvLen = server.Receive(client, buff, 128, false);
                if (recvLen <= 0)
                {
                    break;
                }
                if (recvLen > 0)
                {
                    std::string strReq(buff,recvLen);
                    std::cout << "Recv: " << strReq << std::endl;
                    if (handler.OnClientReq(strReq))
                    {
                    }
                    else
                    {
                        std::cout<<"Handle Client Failed"<<std::endl;
                    }
                }
            }
            server.Disconnect(client);
        }
    }
}